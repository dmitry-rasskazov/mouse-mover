#!/usr/bin/env sh

git clone git@github.com:dmitry-rasskazov/mouse-mover.git /tmp/mouse-mover

gcc /tmp/mouse-mover/mouse-mover.c -o /tmp/mouse-mover/mouse-mover -lX11
cp /tmp/mouse-mover/mouse-mover /usr/bin/mouse-mover
chmod a+x /usr/bin/mouse-mover

echo "[Unit]" > /etc/systemd/system/mouse-mover.service
echo "Description=Mouse mover demon" >> /etc/systemd/system/mouse-mover.service
echo "After=network.target" >> /etc/systemd/system/mouse-mover.service
echo "" >> /etc/systemd/system/mouse-mover.service
echo "[Service]" >> /etc/systemd/system/mouse-mover.service
echo "Type=simple" >> /etc/systemd/system/mouse-mover.service
echo "ExecStart=/usr/bin/mouse-mover" >> /etc/systemd/system/mouse-mover.service
echo "WorkingDirectory=/home/mouse-mover" >> /etc/systemd/system/mouse-mover.service
echo "Restart=on-failure" >> /etc/systemd/system/mouse-mover.service
echo "RestartSec=5s" >> /etc/systemd/system/mouse-mover.service
echo "User=root" >> /etc/systemd/system/mouse-mover.service
echo "Group=root" >> /etc/systemd/system/mouse-mover.service
echo "UMask=077" >> /etc/systemd/system/mouse-mover.service
echo "" >> /etc/systemd/system/mouse-mover.service
echo "[Install]" >> /etc/systemd/system/mouse-mover.service
echo "WantedBy=multi-user.target" >> /etc/systemd/system/mouse-mover.service

systemctl restart mouse-mover.service