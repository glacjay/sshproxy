#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

#include "MainDialog.hpp"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(("SSH Proxy"));

    QLabel *hostLabel = new QLabel(("SSH Host"));
    QLabel *portLabel = new QLabel(("SSH Port"));

#if 0
    QGridLayout *inputLayout = new QGridLayout;
    inputLayout->addWidget(hostLabel, 0, 0);
    inputLayout->addWidget(mHostEdit, 0, 1);
    inputLayout->addWidget(portLabel, 1, 0);
    inputLayout->addWidget(mPortEdit, 1, 1);

    QVBoxLayout *ctrlLayout = new QVBoxLayout;
    ctrlLayout->addLayout(inputLayout);
    ctrlLayout->addSpacerItem(new QSpacerItem(0, 0));

    QGroupBox *ctrlGroup = new QGroupBox(tr("Control"));
    ctrlGroup->setLayout(ctrlLayout);
#endif

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(hostLabel);
    mainLayout->addWidget(portLabel);

    setLayout(mainLayout);
}

MainDialog::~MainDialog(void)
{
}
