#ifndef HUB_DEVICE_H
#define HUB_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include "device_address.h"

class HubDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit HubDevice(QWidget *parent = 0);

private:
    QLabel          *info;
    DeviceAddress   *addr;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;
};

#endif // HUB_DEVICE_H