#ifndef OPERATION_H
#define OPERATION_H

#include <QVector>
#include <QtMath>

template <typename Ts, typename Td> class TypeCastOperation;
template <typename T> class Constant;

#define DEFINE_BINARY_OPERATOR(name, math_operator, output_type)                                \
    template <typename T1, typename T2> class name : public Operation<output_type>              \
    {                                                                                           \
    public:                                                                                     \
        name(Operation<T1>* input1, Operation<T2>* input2){                                     \
            this->input1 = input1;                                                              \
            this->input2 = input2;                                                              \
            mSize = qMin(input1->size(), input2->size());                                       \
        }                                                                                       \
        name(Operation<T1>* input1, const T2& input2){                                          \
            this->input1 = input1;                                                              \
            this->input2 = new Constant<T2>(input1, input2);                                    \
            mSize = input1->size();                                                             \
        }                                                                                       \
        name(const T1& input1, Operation<T2>* input2){                                          \
            this->input1 = new Constant<T1>(input2, input1);                                    \
            this->input2 = input2;                                                              \
            mSize = input2->size();                                                             \
        }                                                                                       \
        virtual int size() const override { return mSize; }                                     \
        virtual output_type at(int i) const override { return input1->at(i) math_operator       \
                                                              input2->at(i);}                   \
        virtual void onUpdated(){                                                               \
            input1->onUpdated();                                                                \
            input2->onUpdated();                                                                \
        }                                                                                       \
        virtual void onLoaded(){                                                                \
            input1->onLoaded();                                                                 \
            input2->onLoaded();                                                                 \
            mSize = qMin(input1->size(), input2->size());                                       \
        }                                                                                       \
        virtual void onAppended(){                                                              \
            input1->onAppended();                                                               \
            input1->onAppended();                                                               \
            mSize = qMin(input1->size(), input2->size());                                       \
        }                                                                                       \
        virtual Operation<output_type>* clone() const override {                                      \
            return new name(input1, input2);                                                    \
        }                                                                                       \
                                                                                                \
    private:                                                                                    \
        Operation<T1>* input1;                                                                  \
        Operation<T2>* input2;                                                                  \
        int mSize = 0;                                                                          \
    };                                                                                          \
    template <typename T1, typename T2>                                                         \
    name<T1, T2> operator math_operator (const Operation<T1>& input1, const Operation<T2>& input2){         \
        return name<T1, T2>(input1.clone(), input2.clone());                                    \
    }

#define DEFINE_UNARY_FUNC(name, func_name, output_type)                                         \
    class name : public Operation<output_type>                                                  \
    {                                                                                           \
    public:                                                                                     \
        name(Operation<output_type>* input){                                                    \
            this->input = input;                                                                \
        }                                                                                       \
        name(Operation<output_type>& input){                                                    \
            this->input = input.clone();                                                        \
        }                                                                                       \
        virtual void onUpdated(){                                                               \
            input->onUpdated();                                                                 \
        }                                                                                       \
        virtual void onLoaded(){                                                                \
            input->onLoaded();                                                                  \
        }                                                                                       \
        virtual void onAppended(){                                                              \
            input->onAppended();                                                                \
        }                                                                                       \
        virtual output_type at(int i) const override { return func_name(input->at(i)); }        \
        virtual int size() const override { return input->size(); }                             \
        virtual Operation<output_type>* clone() const override { return new name(input); }            \
    private:                                                                                    \
        Operation<output_type>* input;                                                          \
    }

#define DEFINE_TEMPLATE_UNARY_FUNC(name, func_name, output_type)                                \
    template <typename T>                                                                       \
    class name : public Operation<output_type>                                                  \
    {                                                                                           \
    public:                                                                                     \
        name(Operation<T>* input){                                                              \
            this->input = input;                                                                \
        }                                                                                       \
        name(const Operation<T>& input){                                                              \
            this->input = input.clone();                                                        \
        }                                                                                       \
        virtual void onUpdated(){                                                               \
            input->onUpdated();                                                                 \
        }                                                                                       \
        virtual void onLoaded(){                                                                \
            input->onLoaded();                                                                  \
        }                                                                                       \
        virtual void onAppended(){                                                              \
            input->onAppended();                                                                \
        }                                                                                       \
        virtual output_type at(int i) const override { return func_name(input->at(i)); }        \
        virtual int size() const override { return input->size(); }                             \
        virtual Operation<output_type>* clone() const override { return new name(input); }            \
    private:                                                                                    \
        Operation<output_type>* input;                                                          \
    }

#define DEFINE_BINARY_FUNC(name, func_name, output_type)                                        \
    template <typename T> class name : public Operation<output_type>                            \
    {                                                                                           \
    public:                                                                                     \
        name(Operation<T>* input1, Operation<T>* input2){                                       \
            this->input1 = input1;                                                              \
            this->input2 = input2;                                                              \
            mSize = qMin(input1->size(), input2->size());                                       \
        }                                                                                       \
        name(const Operation<T>& input1, const Operation<T>& input2){                                       \
            this->input1 = input1.clone();                                                      \
            this->input2 = input2.clone();                                                      \
            mSize = qMin(input1.size(), input2.size());                                         \
        }                                                                                       \
        virtual void onUpdated(){                                                               \
            input1->onUpdated();                                                                \
            input2->onUpdated();                                                                \
        }                                                                                       \
        virtual void onLoaded(){                                                                \
            input1->onLoaded();                                                                 \
            input2->onLoaded();                                                                 \
            mSize = qMin(input1->size(), input2->size());                                       \
        }                                                                                       \
        virtual void onAppended(){                                                              \
            input1->onAppended();                                                               \
            input1->onAppended();                                                               \
            mSize = qMin(input1->size(), input2->size());                                       \
        }                                                                                       \
        virtual output_type at(int i) const override {                                          \
            return func_name(input1->at(i), input2->at(i));                                     \
        }                                                                                       \
        virtual int size() const override { return mSize; }                                     \
        virtual Operation<output_type>* clone() const override {                                      \
            return new name(input1, input2);                                                    \
        }                                                                                       \
    private:                                                                                    \
        Operation<T>* input1;                                                                   \
        Operation<T>* input2;                                                                   \
        int mSize;                                                                              \
    }

/*! \brief ������ֵ���������������ֵ����������������Ϊdouble��Ϊ�˼��ݴ�����������͡�
 *  \warning �޷�����������ֵ������������Ϊģ��������ͣ���Ϊ���ͺ� DEFINE_BINARY_OPERATOR ���Ѿ���
 *           ������������������ͻ���������޷�����(const Operation<T1>&, const Operation<T2>&)����
 *           �б��(const Operation<T1>&, const T2&)�����б�
 */
#define OVERRIDE_NUMERIC_OPERATOR(name, math_operator)                                          \
    template <typename T>                                                                       \
    name<T, double> operator math_operator (const Operation<T>& input1, double t){                    \
        return name<T, double>(input1.clone(), t);                                              \
    }                                                                                           \
    template <typename T>                                                                       \
    name<double, T> operator math_operator (double t, const Operation<T>& input2){                    \
        return name<double, T>(t, input2.clone());                                              \
    }

#define OVERRIDE_LOGIC_OPERATOR(name, logic_operator)                                           \
    template <typename T>                                                                       \
    name<T, T> operator logic_operator (const Operation<T>& input1, const T& input2){                 \
        return name<T, T>(input1.clone(), input2);                                              \
    }                                                                                           \
    template <typename T>                                                                       \
    name<T, T> operator logic_operator (const T& input1, const Operation<T>& input2){                 \
        return name<T, T>(input1, input2.clone());                                              \
    }

/*!
 * \brief �����ʽ�Ե��ӵ�װ������ʽ�洢��
 * \warning ÿ����һ��װ��������һ��������Ͷ�һ���м�������ˣ�Ӧ�þ���������ǰ�Ķ��󣬼����������ʹ洢�ռ�ʹ������
 * \todo �������Ż��ռ䣺��Щ��Ϊ�򵥵����㲻��Ҫ�洢�м�������Щ������Լ��ɵ�һ���ô洢�ռ䡣
 */
template <typename T> class Operation
{
public:
    Operation() {}

    virtual ~Operation() {}

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���������ʽ
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void onUpdated() {}
    virtual void onLoaded() {}
    virtual void onAppended() {}

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // �����������Ϣ
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int size() const = 0;
    bool isEmpty() const { return size() == 0; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���Ʋ���
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual Operation<T>* clone() const = 0;

    /*! \warning �������ͨ�����ظú�����ʵ���Զ����������ܡ�*/
    virtual T at(int i) const = 0;
    const T operator[](int i) const { return at(i); }

    template<typename Td> operator Operation<Td>(){
        return TypeCastOperation<T, Td>(this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���㣨ע�⣺����at()�����Ĳ�ͬʵ�֣����㺯�����ܻ��в�ͬ�ĸ��Ӷȣ���
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    T regionalMinValue(int minIndex, int maxIndex) const
    {
        T result = at(minIndex);
        for(int i = minIndex + 1; i < maxIndex; i++){
            result = qMin(result, at(i));
        }
        return result;
    }

    T regionalMaxValue(int minIndex, int maxIndex) const
    {
        T result = at(minIndex);
        for(int i = minIndex + 1; i < maxIndex; i++){
            result = qMax(result, at(i));
        }
        return result;
    }
};

/*! \brief ����ת��������*/
template <typename Ts, typename Td> class TypeCastOperation : public Operation<Td>
{
public:
    TypeCastOperation(Operation<Ts>* input) { mInput = input; }
    TypeCastOperation(const Operation<Ts>& input) { mInput = input.clone(); }

    /*! \brief ʵ������ת����*/
    virtual Td at(int i) const override { return mInput->at(i); }
    virtual int size() const override { return mInput->size(); }

    virtual Operation<Td>* clone() const override { return new TypeCastOperation<Ts, Td>(mInput); }

private:
    Operation<Ts>* mInput;
};

/*!
 * \brief ���л��幦�ܵĲ��������ݼ���֮��������ʱ�������м����ݡ�
 *
 * BufferedOperation�ļ���ȫ����onLoaded()��onUpdated()�Լ�onAppended()�н��С���������У�������ɺ󣩱�����
 * �����������������ʱ���б�Ҫ���������Ѽ���ģ�����ֵ��С�����ݡ�
 */
template <typename T> class BufferedOperation : public Operation<T>
{
public:
    BufferedOperation() {}
    explicit BufferedOperation(int size) : result(QVector<T>(size)) {}

    BufferedOperation(int size, const T &t) : result(QVector<T>(size, t)) {}
    inline BufferedOperation(const QVector<T> &v) : result(QVector<T>(v)) {}

    /*!
     * \brief ������������ݡ��ú���ʵ���˻��湦�ܣ�����ֱ�������Ѿ�����Ľ�������ݡ�
     */
    virtual T at(int i) const override final { return result[i]; }
    T& operator[](int i) { return result[i]; }
    const T operator[](int i) const { return result[i]; }

    virtual int size() const override final { return result.size(); }

    virtual Operation<T>* clone() const override { return new BufferedOperation(result); }

protected:
    void resize(int size) { result.resize(size); }
    void append(const T& t) { result.append(t); }

private:
    QVector<T> result;
};

/*!
 * \brief Ϊ���߱����湦�ܵļ�����̣���ֱ�Ӽ̳���Operation<T>�Ĳ��������ӻ��湦�ܡ�
 */
template <typename T> class BufferedOperationWrapper : public BufferedOperation<T>
{
public:
    explicit BufferedOperationWrapper(Operation<T>* input) {
        mInput = input;
        load();
    }

    explicit BufferedOperationWrapper(const Operation<T>& input) {
        mInput = input.clone();
        load();
    }

    virtual void onUpdated() override {
        if(mInput){
            mInput->onUpdated();
            (*this)[this->size() - 1] = mInput->at(this->size() - 1);
        }
    }
    virtual void onLoaded() override {
        load();
    }
    virtual void onAppended() override {
        if(mInput){
            mInput->onAppended();
            this->append(mInput->at(this->size()));
        }
    }

    virtual Operation<T>* clone() const override { return new BufferedOperationWrapper(mInput); }

protected:
    void load(){
        if(mInput){
            mInput->onLoaded();
            this->resize(mInput->size());
            for(int i = 0; i < this->size(); i++){
                (*this)[i] = mInput->at(i);
            }
        }
    }

private:
    Operation<T>* mInput = nullptr;
};

/*!
 * \brief ʹ�ø���װ�εĲ���������ʱ�����ظ�ʹ�ã�һ������װ�� BufferedOperation����Ϊֻ�л�������ݲ����ظ�ʹ�ã���
 */
template <typename T> class ReusableOperationWrapper : public Operation<T>
{
public:
    explicit ReusableOperationWrapper(Operation<T> *input, bool operationBuffered = false){
        mInput = operationBuffered ? input : new BufferedOperationWrapper<T>(input);
    }

    explicit ReusableOperationWrapper(const Operation<T>& input, bool operationBuffered = false){
        mInput = operationBuffered ? input.clone() : new BufferedOperationWrapper<T>(input.clone());
    }

    virtual void onUpdated() override {
        mInput->onUpdated();
    }
    virtual void onLoaded() override {
        mInput->onLoaded();
    }
    virtual void onAppended() override {
        mInput->onAppended();
    }

    virtual int size() const override { return mInput->size(); }
    virtual T at(int i) const { return mInput->at(i); }

    /*!
     * \brief Ĭ������£������Ĳ����Ѿ��߱����湦�ܣ�����Ҫ�ٴμ�װ���棬���Խ����� operationBuffered ��Ϊtrue��
     */
    virtual Operation<T>* clone() const override { return new ReusableOperationWrapper<T>(mInput, true); }

private:
    Operation<T>* mInput = nullptr;
};

/*! \brief ��ֵ���㡣*/
DEFINE_BINARY_OPERATOR(PLUS, +, double)
DEFINE_BINARY_OPERATOR(MINUS, -, double)
DEFINE_BINARY_OPERATOR(MULTIPLY, *, double)
DEFINE_BINARY_OPERATOR(DIVIDE, /, double)
OVERRIDE_NUMERIC_OPERATOR(PLUS, +)
OVERRIDE_NUMERIC_OPERATOR(MINUS, -)
OVERRIDE_NUMERIC_OPERATOR(MULTIPLY, *)
OVERRIDE_NUMERIC_OPERATOR(DIVIDE, /)

/*! \brief �߼����㡣*/
DEFINE_BINARY_OPERATOR(GREATER_THAN, >, bool)
DEFINE_BINARY_OPERATOR(GREATER_THAN_OR_EQUAL, >=, bool)
DEFINE_BINARY_OPERATOR(LESS_THAN, <, bool)
DEFINE_BINARY_OPERATOR(LESS_THAN_OR_EQUAL, <=, bool)
DEFINE_BINARY_OPERATOR(EQUAL_TO, ==, bool)
DEFINE_BINARY_OPERATOR(AND, &&, bool)
DEFINE_BINARY_OPERATOR(OR, ||, bool)
OVERRIDE_LOGIC_OPERATOR(GREATER_THAN, >)
OVERRIDE_LOGIC_OPERATOR(GREATER_THAN_OR_EQUAL, >=)
OVERRIDE_LOGIC_OPERATOR(LESS_THAN, <)
OVERRIDE_LOGIC_OPERATOR(LESS_THAN_OR_EQUAL, <=)
OVERRIDE_LOGIC_OPERATOR(EQUAL_TO, ==)
OVERRIDE_LOGIC_OPERATOR(AND, &&)
OVERRIDE_LOGIC_OPERATOR(OR, ||)

/*! \brief ��ѧ������*/
DEFINE_UNARY_FUNC(TAN, qTan, double);
DEFINE_UNARY_FUNC(SIN, qSin, double);
DEFINE_UNARY_FUNC(COS, qCos, double);
DEFINE_UNARY_FUNC(SQRT, qSqrt, double);
DEFINE_UNARY_FUNC(LN, qLn, double);
DEFINE_UNARY_FUNC(EXP, qExp, double);
DEFINE_TEMPLATE_UNARY_FUNC(ABS, qAbs, T);

DEFINE_BINARY_FUNC(MIN, qMin, T);
DEFINE_BINARY_FUNC(MAX, qMax, T);

/*! \brief ��ǰ����ֵ��*/
template <typename T> class INDEX : public Operation<int>
{
public:
    INDEX(Operation<T>* user) { mUser = user; }
    INDEX(Operation<T>& user) { mUser = user.clone(); }

    virtual int size() const { return mUser->size(); }
    virtual int at(int i) const override { return i; }

    virtual Operation<int>* clone() const override { return new INDEX(mUser); }

private:
    Operation<T>* mUser;
};

/*! \brief ȡ�෴��������*/
template <typename T> MINUS<T, T> operator-(Operation<T>& input)
{
    Operation<T>* i = input.clone();
    return MINUS<T, T>(new Constant<T>(i, T()), i);
}

/*! \brief ��ʾһ����ֵ�������������ĳ��Ⱥ��Զ�����һ�������Ľ�����ȱ���һ�¡�*/
template <typename T> class Constant : public Operation<T>
{
public:
    Constant(Operation<T>* user, const T &t) {  mUser = user, mValue = t; }

    virtual int size() const { return mUser->size(); }

    virtual T at(int i) const override { Q_UNUSED(i); return mValue; }

    virtual Operation<T>* clone() const override { return new Constant<T>(mUser, mValue); }

private:
    Operation<T>* mUser;
    T mValue;
};

/*! \brief ��������ǽ���������������һ����Χ�ڵĽ����*/
template <typename T> class BOUND : public Operation<T>
{
public:
    BOUND(Operation<T>* lower, Operation<T>* input, Operation<T>* upper):
        mUpper(upper),
        mLower(lower),
        mInput(input)
    {
    }

    BOUND(const T& lower, Operation<T>* input, const T& upper):
        mUpper(new Constant<T>(input, upper)),
        mLower(new Constant<T>(input, lower)),
        mInput(input)
    {
    }

    BOUND(const T& lower, const Operation<T>& input, const T& upper)
    {
        Operation<T>* c = input.clone();
        mUpper = new Constant<T>(c, upper);
        mLower = new Constant<T>(c, lower);
        mInput = c;
    }

    virtual void onUpdated() {
        mUpper->onUpdated();
        mLower->onUpdated();
        mInput->onUpdated();
    }
    virtual void onLoaded() {
        mUpper->onLoaded();
        mLower->onLoaded();
        mInput->onLoaded();
    }
    virtual void onAppended() {
        mUpper->onAppended();
        mLower->onAppended();
        mInput->onAppended();
    }

    virtual int size() const { return mInput->size(); }
    virtual T at(int i) const override { return qBound(mLower->at(i), mInput->at(i), mUpper->at(i)); }

    virtual Operation<T>* clone() const override { return new BOUND<T>(mLower, mInput, mUpper); }

private:
    Operation<T>* mUpper;
    Operation<T>* mLower;
    Operation<T>* mInput;
};

/*! \brief ������������֮ǰ�����ݡ�*/
template <typename T> class REF : public Operation<T>
{
public:
    REF(Operation<T>* input, int n) { mInput = input; mPeriod = n; }
    REF(Operation<T>& input, int n) : REF(input.clone(), n) {}

    virtual void onUpdated() { mInput->onUpdated(); }
    virtual void onLoaded() { mInput->onLoaded(); }
    virtual void onAppended() { mInput->onAppended(); }

    virtual int size() const { return mInput->size(); }
    virtual T at(int i) const override { return i >= mPeriod ? mInput->at(i - mPeriod) : mInput->at(0); }

    virtual Operation<T>* clone() const override { return new REF(mInput, mPeriod); }

private:
    Operation<T>* mInput;
    int mPeriod;
};

/*! \brief �൱��C++����Ŀ�����"[condition] ? [result1] : [result2]"��*/
template <typename T> class IF : public Operation<T>
{
public:
    IF(Operation<bool>* condition, Operation<T>* input1, Operation<T>* input2) {
        mInput1 = input1;
        mInput2 = input2;
        mCondition = condition;
    }
    IF(const Operation<bool>& condition, const Operation<T>& input1, const Operation<T>& input2) :
    IF(condition.clone(), input1.clone(), input2.clone()) {}

    IF(const Operation<bool>& condition, const Operation<T>& input1, const T& t) {
        mCondition = condition.clone();
        mInput1 = input1.clone();
        mInput2 = new Constant<T>(mInput1, t);
    }

    IF(const Operation<bool>& condition, const T& t, const Operation<T>& input2) {
        mCondition = condition.clone();
        mInput2 = input2.clone();
        mInput1 = new Constant<T>(mInput2, t);
    }

    virtual void onUpdated() override {
        mCondition->onUpdated();
        mInput1->onUpdated();
        mInput2->onUpdated();
        mSize = qMin(mCondition->size(), qMin(mInput1->size(), mInput2->size()));
    }
    virtual void onLoaded() override {
        mCondition->onLoaded();
        mInput1->onLoaded();
        mInput2->onLoaded();
        mSize = qMin(mCondition->size(), qMin(mInput1->size(), mInput2->size()));
    }
    virtual void onAppended() override {
        mCondition->onAppended();
        mInput1->onAppended();
        mInput2->onAppended();
        mSize = qMin(mCondition->size(), qMin(mInput1->size(), mInput2->size()));
    }

    virtual T at(int i) const override { return mCondition->at(i) ? mInput1->at(i) : mInput2->at(i); }
    virtual int size() const { return mSize; }

    virtual Operation<T>* clone() const override { return new IF(mCondition, mInput1, mInput2); }

private:
    Operation<bool>* mCondition;
    Operation<T>* mInput1;
    Operation<T>* mInput2;
    int mSize;
};

/*! \brief ��͡�֧������ģʽ��������Ϊ0��ʾ�ۼ���ͣ����ڴ������ʾ�������ƶ���͡�*/
template <typename T> class SUM : public BufferedOperation<T>
{
public:
    SUM(Operation<T>* input, int period) {
        mInput = input;
        mPeriod = period;
    }

    SUM(const Operation<double>& input, int period) : SUM(input.clone(), period) {}

    virtual void onUpdated() override {
        mInput->onUpdated();

        int index = this->size() - 1;

        if(mPeriod == 0){
            if(this->size() == 1)
                (*this)[index] = mInput->at(index);
            else
                (*this)[index] = (*this)[index - 1] + mInput->at(index);
        }else{
            if(this->size() >= mPeriod){
                (*this)[index] = (*this)[index - 1] + mInput->at(index) - mInput->at(index - mPeriod);
            }else{
                (*this)[index] = (*this)[index - 1] + mInput->at(index) - mInput->at(0);
            }
        }
    }
    virtual void onLoaded() override {
        mInput->onLoaded();
        this->resize(mInput->size());
        if(this->isEmpty()) return;

        if(mPeriod == 0){
            (*this)[0] = mInput->at(0);
            for(int i = 1; i < this->size(); i++){
                (*this)[i] = (*this)[i - 1] + mInput->at(i);
            }
        }else{
            (*this)[0] = mInput->at(0) * mPeriod;
            for(int i = 1; i < mPeriod; i++){
                (*this)[i] = (*this)[i - 1] + mInput->at(i) - mInput->at(0);
            }
            for(int i = mPeriod; i < this->size(); i++){
                (*this)[i] = (*this)[i - 1] + mInput->at(i) - mInput->at(i - mPeriod);
            }
        }
    }
    virtual void onAppended() override {
        int index = mInput->size() - 1;
        mInput->onAppended();

        if(mPeriod == 0){
            this->append(this->at(index) + mInput->at(index + 1));
        }else{
            if(index == 0){
                this->append(mInput->at(0) * mPeriod);
            }else if(index <= mPeriod - 1){
                this->append(this->at(index - 1) + mInput->at(index) - mInput->at(0));
            }else{
                this->append(this->at(index - 1) + mInput->at(index) - mInput->at(index - mPeriod));
            }
        }
    }

    virtual Operation<T>* clone() const override { return new SUM(mInput, mPeriod); }

private:
    Operation<T>* mInput;
    T mPervious = 0.0;
    int mPeriod;
};

/*! \brief �˷����㡣*/
class POW : public Operation<double>
{
public:
    POW(Operation<double>* input, double n) {
        mInput = input;
        mExponent = n;
    }
    POW(const Operation<double>& input, double n) : POW(input.clone(), n) {}

    virtual void onUpdated() override {
        mInput->onUpdated();
    }
    virtual void onLoaded() override {
        mInput->onLoaded();
    }
    virtual void onAppended() override {
        mInput->onAppended();
    }

    virtual double at(int i) const override {
        return pow(mInput->at(i), mExponent);
    }

    virtual int size() const { return mInput->size(); }

    virtual Operation<double>* clone() const override { return new POW(mInput, mExponent); }

private:
    Operation<double>* mInput;
    double mExponent;
};

/*! \brief ����ƽ����*/
class MA : public Operation<double>
{
public:
    MA(Operation<double>* input, int n){
        mInput = input;
        mPeriod = new Constant<double>(mInput, n);
        mOutput = new DIVIDE<double, double>(new SUM<double>(mInput, n), mPeriod);
    }

    MA(const Operation<double>& input, int n) : MA(input.clone(), n) {}

    virtual void onUpdated() override {
        mPeriod->onUpdated();
        mInput->onUpdated();
        mOutput->onUpdated();
    }
    virtual void onLoaded() override {
        mInput->onLoaded();
        mOutput->onLoaded();
    }
    virtual void onAppended() override {
        mInput->onAppended();
        mOutput->onAppended();
    }

    virtual double at(int i) const override { return mOutput->at(i); }
    virtual int size() const { return mInput->size(); }

    virtual Operation<double>* clone() const override { return new MA(mInput, mPeriod->at(0)); }

private:
    Constant<double>* mPeriod;
    Operation<double>* mInput;
    Operation<double>* mOutput;
};

/*! \brief ָ���ƶ�ƽ����*/
class SMA : public BufferedOperation<double>
{
public:
    SMA(Operation<double>* input, int n, double weight){
        mInput = input;
        mPeriod = n;
        mWeight = weight;
    }
    SMA(const Operation<double>& input, int n, double weight){
        mInput = input.clone();
        mPeriod = n;
        mWeight = weight;
    }

    virtual void onUpdated() override {
        mInput->onUpdated();
        if(size() == 1){
            (*this)[0] = mInput->at(0);
            return;
        }
        (*this)[size() - 1] = mInput->at(size() - 1) * (mWeight/mPeriod) +
                              at(size() - 2) * (1.0 - mWeight/mPeriod);

    }
    virtual void onLoaded() override {
        mInput->onLoaded();
        resize(mInput->size());

        if(this->isEmpty()) return;

        (*this)[0] = mInput->at(0);
        for(int i = 1; i<size(); i++){
            (*this)[i] = mInput->at(i) * (mWeight/mPeriod) + at(i - 1) * (1.0 - mWeight/mPeriod);
        }
    }
    virtual void onAppended() override {
        mInput->onAppended();
        append(mInput->at(size()) * (mWeight/mPeriod) + at(size() - 1) * (1.0 - mWeight/mPeriod));
    }

    virtual Operation<double>* clone() const override { return new SMA(mInput, mPeriod, mWeight); }

private:
    Operation<double>* mInput;
    int mPeriod;
    double mWeight;
};

/*! \brief �ȼ��� SMA(input, n+1, 2)��*/
class EMA : public BufferedOperation<double>
{
public:
    EMA(Operation<double>* input, int n){
        mInput = input;
        mPeriod = n;
        load();
    }

    EMA(const Operation<double>& input, int n){
        mInput = input.clone();
        mPeriod = n;
        load();
    }

    virtual void onUpdated() override {
        mInput->onUpdated();
        if(size() == 1){
            (*this)[0] = mInput->at(0);
            return;
        }
        (*this)[size() - 1] = mInput->at(size() - 1) * (2.0/(mPeriod + 1)) +
                               at(size() - 2)  * (1.0 - 2.0/(mPeriod + 1));
    }
    virtual void onLoaded() override {
        mInput->onLoaded();
        load();
    }
    virtual void onAppended() override {
        mInput->onAppended();
        append(mInput->at(size() - 1) * (2.0/(mPeriod + 1)) + at(size() - 2) * (1.0 - 2.0/(mPeriod + 1)));
    }

    virtual Operation<double>* clone() const override { return new EMA(mInput, mPeriod); }

protected:
    void load(){
        resize(mInput->size());

        if(this->isEmpty()) return;

        (*this)[0] = mInput->at(0);
        for(int i = 1; i<size(); i++){
            (*this)[i] = mInput->at(i) * (2.0/(mPeriod + 1)) + at(i - 1) * (1.0 - 2.0/(mPeriod + 1));
        }
    }

private:
    Operation<double>* mInput;
    int mPeriod;
};

/*! \brief һ�������ڵ����ֵ��*/
template <typename T> class HHV : public Operation<T>
{
public:
    HHV(Operation<T>* input, int period) {
        mInput = input;
        mPeriod = period;
    }

    HHV(Operation<T>& input, int period) {
        mInput = input.clone();
        mPeriod = period;
    }

    virtual void onUpdated() override {
        mInput->onUpdated();
    }
    virtual void onLoaded() override {
        mInput->onLoaded();
    }
    virtual void onAppended() override {
        mInput->onAppended();
    }

    virtual T at(int i) const override {
        T result = mInput->at(i);
        if(i >= mPeriod){
            for(int j = 1; j < mPeriod; j++){
                result = qMax(result, mInput->at(i - j));
            }
        }else{
            for(int j = i + 1; j >= 0; j--){
                result = qMax(result, mInput->at(j));
            }
        }
        return result;
    }
    virtual int size() const { return mInput->size(); }

    virtual Operation<T>* clone() const override { return new HHV(mInput, mPeriod); }

private:
    Operation<T>* mInput;
    int mPeriod;
};

/*! \brief һ�������ڵ���Сֵ��*/
template <typename T> class LLV : public Operation<T>
{
public:
    LLV(Operation<T>* input, int period) {
        mInput = input;
        mPeriod = period;
    }

    LLV(Operation<T>& input, int period) {
        mInput = input.clone();
        mPeriod = period;
    }

    virtual void onUpdated() override {
        mInput->onUpdated();
    }
    virtual void onLoaded() override {
        mInput->onLoaded();
    }
    virtual void onAppended() override {
        mInput->onAppended();
    }

    virtual T at(int i) const override {
        T result = mInput->at(i);
        if(i >= mPeriod){
            for(int j = 1; j < mPeriod; j++){
                result = qMin(result, mInput->at(i - j));
            }
        }else{
            for(int j = i + 1; j >= 0; j--){
                result = qMin(result, mInput->at(j));
            }
        }
        return result;
    }

    virtual int size() const { return mInput->size(); }

    virtual Operation<T>* clone() const override { return new LLV(mInput, mPeriod); }

private:
    Operation<T>* mInput;
    int mPeriod;
};

/*! \brief ��׼�*/
class STD : public Operation<double>
{
public:
    STD(Operation<double>* input, int period) {
        mInput = input;
        mPeriod = period;

        ReusableOperationWrapper<double> i(input, true);

        mOutput = new POW((SUM<double>(POW(i - MA(i, mPeriod), 2), mPeriod) / mPeriod ), 0.5);
    }

    STD(const Operation<double>& input, int period) : STD(input.clone(), period) {}

    virtual void onUpdated() override {
        mInput->onUpdated();
        mOutput->onUpdated();
    }
    virtual void onLoaded() override {
        mInput->onLoaded();
        mOutput->onLoaded();
    }
    virtual void onAppended() override {
        mInput->onAppended();
        mOutput->onAppended();
    }

    virtual double at(int i) const override {
        return mOutput->at(i);
    }

    virtual int size() const { return mInput->size(); }

    virtual Operation<double>* clone() const override { return new STD(mInput, mPeriod); }

private:
    Operation<double>* mInput;
    Operation<double>* mOutput;
    int mPeriod;
};

/*! \brief �;�ֵ�Ĺ����ʡ�*/
class BIAS : public Operation<double>
{
public:
    BIAS(Operation<double>* input, int period) {
        mInput = input;
        mPeriod = period;

        ReusableOperationWrapper<double> i(input, true);
        mOutput = ((i - MA(input, mPeriod)) / MA(input, mPeriod) * 100.0).clone();
    }

    BIAS(Operation<double>& input, int period) : BIAS(input.clone(), period) {}

    virtual void onUpdated() override {
        mInput->onUpdated();
        mOutput->onUpdated();
    }
    virtual void onLoaded() override {
        mInput->onLoaded();
        mOutput->onLoaded();
    }
    virtual void onAppended() override {
        mInput->onAppended();
        mOutput->onAppended();
    }

    virtual double at(int i) const override {
        return mOutput->at(i);
    }

    virtual int size() const { return mInput->size(); }

    virtual Operation<double>* clone() const override { return new BIAS(mInput, mPeriod); }

private:
    Operation<double>* mInput;
    Operation<double>* mOutput;
    int mPeriod;
};

#endif // OPERATION_H
