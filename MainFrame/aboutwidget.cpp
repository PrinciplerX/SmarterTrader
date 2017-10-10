#include "aboutwidget.h"
#include "mainframe_global.h"
#include <QPicture>

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    logoLabel(new QLabel()),
    appLabel(new QLabel(QString("SuperTrader 0.4.0"))),
    textEdit(new QTextEdit())
{
    QPixmap pixmap(":/AppIcon.ico");
    pixmap = pixmap.scaled(48, 48);

    logoLabel->setPixmap(pixmap);
    QFont font= appLabel->font();
    font.setPointSize(24);
    font.setBold(true);
    appLabel->setFont(font);

    initTextEdit(textEdit);

    logoLabel->setAlignment(Qt::AlignCenter);
    appLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout* logoLayout = new QHBoxLayout();
    logoLayout->addWidget(logoLabel);
    logoLayout->addWidget(appLabel);
    logoLayout->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(logoLayout);
    mainLayout->addWidget(textEdit);
}

void AboutWidget::initTextEdit(QTextEdit *edit)
{
    edit->setReadOnly(true);
    edit->setText(QString(">> �����һ�汾���ð�ʵ�ֵ��¹��ܣ�\n\n"
                          "1. ��ԭ�е��������ģ�����MVCģʽ���죬��View��Model���룬�ṩ����Qt Model/View����"
                          "֧������ʵʱ��̬���µĹ��ܺͽӿڡ�\n\n"
                          "2. ʵ�ֹ�Ʊ�۸�����ͼ��ʵʱ���º��л���ͬһ������ͼ�ؼ������л���ʾ�����Ʊ����ͼ������"
                          "��Ʊ���ݺ�����ͼ�ؼ����׷��롣ʵ�ֹ�Ʊ����Model������ͼView�Ķ�̬��ϡ�\n\n"
                          "3. ʵ���µ�ָ����㷽ʽ��Ϊ�Ժ�ʵ�ֹ�ʽ�ű�����ָ����»�����\n\n"
                          "4. ���ƹ�Ʊ���ݽӿڣ������������������ļ������0.2.0�汾��Ҫ��ticks.csv��table.csv��,"
                          "��������ֱ��������ѯ��\n\n"
                          "5. ʵ�ֹ�Ʊ��ʷ��K�ߣ����ݣ����շֱʣ�tick�����ݣ���ʷ�ֱ����ݣ��嵵���ݵĻ�ȡ�ͽ������ܡ�\n\n"
                          "6. ���������ǰ��衣�����������������������Ʊ��\n\n"
                          "7. ʵ���տ����˲ƾ����ŵĹ��ܡ�\n\n"
                          "8. ʵ�ֲ鿴������Ĺ��ܡ�\n\n"
                          "9. �����˿�ѡ�Ľ�����\n\n"
                          ">> �������Ͳο����Ŀ�Դ��Ŀ��Qwt��Qt Charts��Tushare��\n"
                          "��ӯ(��)��  �ּ�/�����ȫ���ÿ������"
                          "���ȣ�      ���к�ÿ���ӵ�ƽ���ɽ������ȥ5��������ÿ����ƽ���ɽ���֮��"
                          "��ͨ��ֵ��  ��ͨ�ɱ�*�ּ�"
                          "ϸ����ҵ"
                          "����"
                          "��������"
                          "3���Ƿ�"


                          "�ܹɱ���    ��˾���ܹɱ�"
                          "����ͬ�ȣ�  (���ڵ�����-ȥ��ͬ�ڵ�����) /ȥ��ͬ�ڵ�����*100"
                          "����ͬ�ȣ�  (���ڵ�����-ȥ��ͬ�ڵ�����) /ȥ��ͬ�ڵ�����*100"
                          "�о��ʣ�    ����PB���ּ�/ÿ�ɾ��ʲ�"
                          "�����ʣ�    �ּ�/ÿ���ֽ�����"
                          "�����ʣ�    �ּ�/ÿ��Ӫҵ����"
                          "ÿ�����棺  ���ڵľ�����/�ܹɱ�"
                          "ÿ�ɾ��ʣ�  ���ʲ�/�ܹɱ�"
                          "ÿ�ɹ�����  ������/�ܹɱ�"
                          "ÿ��δ���䣺δ��������/�ܹɱ�"
                          "Ȩ��ȣ�    ���ʲ�/���ʲ�*100"
                          "�����ʣ�    �����ʲ�������,���ڵľ�����/���ʲ�*100"
                          "����ë���ʣ�(Ӫҵ����-Ӫҵ�ɱ�) /Ӫҵ����*100"
                          "Ӫҵ�����ʣ�Ӫҵ����/Ӫҵ����*100"
                          "�������ʣ�  ������/Ӫҵ����*100"

                          "��ӯ(TTM):  �������12���µ�ÿ������õ�����ӯ��"
                          "��ӯ(��):   ��̬��ӯ��(����걨����ӯ��)"
                          "����ϵ��:   ��������ڴ��̵Ĳ�������(���60��ͳ��),�����ֵԽ��,��ʾ������仯��������ڴ��̵ı仯����Խ��;����Ǹ�ֵ,���ʾ�����෴.0.5Ϊ�ͷ��չ�Ʊ,1.0��ʾΪƽ�����չ�Ʊ,2.0�߷��չ�Ʊ,�������Ʊ����0.5��l.5��"
                          "�������"
                          "���ʲ�"
                          "���ʲ�"
                          "������Ȩ"
                          "�ʲ���ծ��%"
                          "�����ʲ� �̶��ʲ� �����ʲ� ������ծ ������ ��� Ӧ���˿� Ӫҵ���� Ӫҵ�ɱ� Ӫҵ���� Ͷ������ �����ܶ� ˰������ ������ δ������ ��Ӫ�ֽ��� ���ֽ��� �ɶ����� �˾���ֵ"
                          ""));
}
