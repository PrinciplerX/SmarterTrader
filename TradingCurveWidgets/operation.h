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

/*! \brief 重载数值运算符。将单个数值参数的数据类型设为double是为了兼容大多数数据类型。
 *  \warning 无法将单独的数值的数据类型设为模板参数类型，因为这会和宏 DEFINE_BINARY_OPERATOR 中已经定
 *           义的重载运算符函数冲突：编译器无法区别(const Operation<T1>&, const Operation<T2>&)参数
 *           列表和(const Operation<T1>&, const T2&)参数列表。
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
 * \brief 将表达式以叠加的装饰器形式存储。
 * \warning 每创建一个装饰器叠加一层操作，就多一个中间结果。因此，应该尽量重用以前的对象，减少运算量和存储空间使用量。
 * \todo 这里有优化空间：有些较为简单的运算不需要存储中间结果，有些运算可以集成到一起共用存储空间。
 */
template <typename T> class Operation
{
public:
    Operation() {}

    virtual ~Operation() {}

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 计算操作方式
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void onUpdated() {}
    virtual void onLoaded() {}
    virtual void onAppended() {}

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 操作结果集信息
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int size() const = 0;
    bool isEmpty() const { return size() == 0; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 复制操作
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual Operation<T>* clone() const = 0;

    /*! \warning 子类可以通过重载该函数来实现自定义索引功能。*/
    virtual T at(int i) const = 0;
    const T operator[](int i) const { return at(i); }

    template<typename Td> operator Operation<Td>(){
        return TypeCastOperation<T, Td>(this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 运算（注意：根据at()函数的不同实现，运算函数可能会有不同的复杂度）。
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

/*! \brief 类型转化操作。*/
template <typename Ts, typename Td> class TypeCastOperation : public Operation<Td>
{
public:
    TypeCastOperation(Operation<Ts>* input) { mInput = input; }
    TypeCastOperation(const Operation<Ts>& input) { mInput = input.clone(); }

    /*! \brief 实现类型转化。*/
    virtual Td at(int i) const override { return mInput->at(i); }
    virtual int size() const override { return mInput->size(); }

    virtual Operation<Td>* clone() const override { return new TypeCastOperation<Ts, Td>(mInput); }

private:
    Operation<Ts>* mInput;
};

/*!
 * \brief 具有缓冲功能的操作，数据加载之后计算完成时将保存中间数据。
 *
 * BufferedOperation的计算全部在onLoaded()、onUpdated()以及onAppended()中进行。计算过程中（或者完成后）必须手
 * 动保存计算结果。计算时如有必要可以引用已计算的（索引值较小）数据。
 */
template <typename T> class BufferedOperation : public Operation<T>
{
public:
    BufferedOperation() {}
    explicit BufferedOperation(int size) : result(QVector<T>(size)) {}

    BufferedOperation(int size, const T &t) : result(QVector<T>(size, t)) {}
    inline BufferedOperation(const QVector<T> &v) : result(QVector<T>(v)) {}

    /*!
     * \brief 索引结果集数据。该函数实现了缓存功能，所以直接引用已经缓存的结果集数据。
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
 * \brief 为不具备缓存功能的计算过程（即直接继承自Operation<T>的操作）增加缓存功能。
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
 * \brief 使用该类装饰的操作在运算时将被重复使用（一般用于装饰 BufferedOperation，因为只有缓存的数据才能重复使用）。
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
     * \brief 默认情况下，包裹的操作已经具备缓存功能，不需要再次加装缓存，所以将参数 operationBuffered 设为true。
     */
    virtual Operation<T>* clone() const override { return new ReusableOperationWrapper<T>(mInput, true); }

private:
    Operation<T>* mInput = nullptr;
};

/*! \brief 数值运算。*/
DEFINE_BINARY_OPERATOR(PLUS, +, double)
DEFINE_BINARY_OPERATOR(MINUS, -, double)
DEFINE_BINARY_OPERATOR(MULTIPLY, *, double)
DEFINE_BINARY_OPERATOR(DIVIDE, /, double)
OVERRIDE_NUMERIC_OPERATOR(PLUS, +)
OVERRIDE_NUMERIC_OPERATOR(MINUS, -)
OVERRIDE_NUMERIC_OPERATOR(MULTIPLY, *)
OVERRIDE_NUMERIC_OPERATOR(DIVIDE, /)

/*! \brief 逻辑运算。*/
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

/*! \brief 数学函数。*/
DEFINE_UNARY_FUNC(TAN, qTan, double);
DEFINE_UNARY_FUNC(SIN, qSin, double);
DEFINE_UNARY_FUNC(COS, qCos, double);
DEFINE_UNARY_FUNC(SQRT, qSqrt, double);
DEFINE_UNARY_FUNC(LN, qLn, double);
DEFINE_UNARY_FUNC(EXP, qExp, double);
DEFINE_TEMPLATE_UNARY_FUNC(ABS, qAbs, T);

DEFINE_BINARY_FUNC(MIN, qMin, T);
DEFINE_BINARY_FUNC(MAX, qMax, T);

/*! \brief 当前索引值。*/
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

/*! \brief 取相反数操作。*/
template <typename T> MINUS<T, T> operator-(Operation<T>& input)
{
    Operation<T>* i = input.clone();
    return MINUS<T, T>(new Constant<T>(i, T()), i);
}

/*! \brief 表示一个定值向量。该向量的长度和自动和另一个操作的结果长度保持一致。*/
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

/*! \brief 输出数据是将输入数据限制在一定范围内的结果。*/
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

/*! \brief 引用所设周期之前的数据。*/
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

/*! \brief 相当于C++的三目运算符"[condition] ? [result1] : [result2]"。*/
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

/*! \brief 求和。支持两种模式：周期设为0表示累加求和，周期大于零表示定周期移动求和。*/
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

/*! \brief 乘方运算。*/
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

/*! \brief 算术平均。*/
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

/*! \brief 指数移动平均。*/
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

/*! \brief 等价于 SMA(input, n+1, 2)。*/
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

/*! \brief 一段周期内的最大值。*/
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

/*! \brief 一段周期内的最小值。*/
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

/*! \brief 标准差。*/
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

/*! \brief 和均值的乖离率。*/
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
