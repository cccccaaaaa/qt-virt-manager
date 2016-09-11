#include "chain_rules.h"

ChainRules::ChainRules(QWidget *parent) :
    _QWidget(parent)
{
    prot = new QLabel("Protocol:", this);
    prior = new QLabel("Priority:", this);
    chainProtocol = new QComboBox(this);
    connect(chainProtocol, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changePriorityDefault(int)));
    priority = new QSpinBox(this);
    priority->setRange(-1000, 1000);
    priority->clear();
    paramsLayout = new QHBoxLayout(this);
    paramsLayout->addWidget(prot, 0, Qt::AlignRight);
    paramsLayout->addWidget(chainProtocol, 0, Qt::AlignLeft);
    paramsLayout->addWidget(prior, 0, Qt::AlignRight);
    paramsLayout->addWidget(priority, 0, Qt::AlignLeft);
    paramsWdg = new QWidget(this);
    paramsWdg->setLayout(paramsLayout);
    addRule  = new QPushButton(
                QIcon::fromTheme("list-add"),
                "",
                this);
    addRule->setToolTip("New rule");
    editRule = new QPushButton(
                QIcon::fromTheme("configure"),
                "",
                this);
    editRule->setToolTip("Edit selected rule");
    delRule  = new QPushButton(
                QIcon::fromTheme("list-remove"),
                "",
                this);
    delRule->setToolTip("Delete selected rule");
    ruleButtonsLayout = new QHBoxLayout(this);
    ruleButtonsLayout->addWidget(addRule);
    ruleButtonsLayout->addWidget(editRule);
    ruleButtonsLayout->addWidget(delRule);
    ruleButtons = new QWidget(this);
    ruleButtons->setLayout(ruleButtonsLayout);
    ruleList = new QListWidget(this);
    connect(addRule, SIGNAL(clicked(bool)),
            this, SLOT(addRuleToList()));
    connect(editRule, SIGNAL(clicked(bool)),
            this, SLOT(editRuleInList()));
    connect(delRule, SIGNAL(clicked(bool)),
            this, SLOT(delRuleFromList()));
    chainLayout = new QVBoxLayout(this);
    chainLayout->addWidget(paramsWdg);
    chainLayout->addWidget(ruleList);
    chainLayout->addWidget(ruleButtons);
    chainWdg = new QWidget(this);
    chainWdg->setLayout(chainLayout);

    ruleWdg = new RuleInstance(this);
    connect(ruleWdg, SIGNAL(ruleCancelled()),
            this, SLOT(turnToChainWdg()));
    connect(ruleWdg, SIGNAL(insertRule(const QString&, int)),
            this, SLOT(insertRuleToList(const QString&, int)));

    commonWdg = new QStackedWidget(this);
    commonWdg->addWidget(chainWdg);
    commonWdg->addWidget(ruleWdg);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(commonWdg);
    setLayout(commonLayout);

    // order is template for attributes widgets
    chainProtocol->addItem("STP", "stp");
    chainProtocol->addItem("MAC", "mac");
    chainProtocol->addItem("VLAN", "vlan");
    chainProtocol->addItem("IPv4", "ipv4");
    chainProtocol->addItem("IPv6", "ipv6");
    chainProtocol->addItem("ARP", "arp");
    chainProtocol->addItem("RARP", "rarp");
    chainProtocol->addItem("MIXED", "mixed");
}
void ChainRules::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _filter;
    _filter = doc.firstChildElement("filter");
    if ( !_filter.isNull() ) {
        QString _chain = _filter.attribute("chain");
        int idx = chainProtocol->findData(_chain);
        if ( idx<0 ) idx = chainProtocol->count()-1;
        chainProtocol->setCurrentIndex(idx);
        QString _prior = _filter.attribute("priority");
        priority->setValue(_prior.toInt());
        QDomNode _n = _filter.firstChild();
        while ( !_n.isNull() ) {
            QDomElement _el = _n.toElement();
            if ( !_el.isNull() ) {
                if ( _el.tagName()=="rule" ) {
                    QString _act, _direction, _priority;
                    _act = _el.attribute("action");
                    _direction = _el.attribute("direction");
                    _priority = _el.attribute("priority");
                    ruleList->addItem(QString("%1\t%2\t%3")
                                      .arg(_act)
                                      .arg(_direction)
                                      .arg(_priority));
                };
            };
            _n = _n.nextSibling();
        };
    };
}

/* private slots */
void ChainRules::changePriorityDefault(int i)
{
    int value = 500; // default
    switch (i) {
    case 0:
        value = -810;
        break;
    case 1:
        value = -800;
        break;
    case 2:
        value = -750;
        break;
    case 3:
        value = -700;
        break;
    case 4:
        value = -600;
        break;
    case 5:
        value = -500;
        break;
    case 6:
        value = -400;
        break;
    case 7:
    default:
        break;
    };
    priority->setValue(value);
    ruleList->clear();
    ruleWdg->setAttributesMapByProtocol(i);
}
void ChainRules::addRuleToList()
{
    ruleWdg->editRule("", -1);
    commonWdg->setCurrentWidget(ruleWdg);
}
void ChainRules::editRuleInList()
{
    QList<QListWidgetItem*> l = ruleList->selectedItems();
    if ( l.isEmpty() || l.at(0)==nullptr ) return;
    QListWidgetItem *item = l.at(0);
    int row = ruleList->row(item);
    ruleWdg->editRule(item->data(Qt::UserRole).toString(), row);
    commonWdg->setCurrentWidget(ruleWdg);
}
void ChainRules::delRuleFromList()
{
    QList<QListWidgetItem*> l = ruleList->selectedItems();
    if ( l.isEmpty() || l.at(0)==nullptr ) return;
    QListWidgetItem *item = l.at(0);
    int row = ruleList->row(item);
    ruleList->takeItem(row);
    delete item;
    item = nullptr;
}
void ChainRules::turnToChainWdg()
{
    commonWdg->setCurrentWidget(chainWdg);
}
void ChainRules::insertRuleToList(const QString &_rule, int row)
{
    QListWidgetItem *item;
    if ( row<ruleList->count()-1 ) {
        item = ruleList->takeItem(row);
        delete item;
        item = nullptr;
    };
    item = new QListWidgetItem(ruleList);
    // item set text, data
    ruleList->insertItem(row, item);
    commonWdg->setCurrentWidget(chainWdg);
    // check priority, move to correct place
}
