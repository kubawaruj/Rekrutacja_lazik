Cześć jestem Kuba.

To repozytorium jest jako jednen z projektow workspace'u. Najlepiej umiescic je w "catkin_ws/src"
Pracowałem na wersji ROS noetic 1.15.14

Opis zadania:
Stworzyłem dwa node'y "Generator_node" i "Converter_node". Ten pierwszy jest wsparciem do testów.

Głównym node'm jest Converter_node. Jego zadanie realizuje plik "src/Convert_to_CAN.cpp".
Opis umieściłem w programie. W skrócie, jest to jednocześnie Publisher i Subscriber. 
Odbiera dane z topicu "rover/status". Odebraną wiadomość przesyła na interfejs can0, 
za pomocą biblioteki "ros_canopen" przez topic "sent_messages".

Generator_node jest tylko Publisherem. Jego zadanie realizuje plik "src/Generator.cpp".
Wysyła na przemian status lazika na topic "rover/status":
	0 -> Sterowanie manualne
	1 -> Autonomia
	2 -> Osiągnięto cel
	
Topic "rover/status", korzysta z mojej własnej wiadomości "my_msg", 
która ma jedno pole uint8, nazwane LED_status. 
Dopiero po stworzeniu działąjącego programu, przeczytałem/zrozumiałem, 
że nie musiałem tworzyć tej wiadomości, gdyż mogłem użyć domyślnej wiadomości uint8.


Opis testów:
Po pierwsze, korzystałem z Ubuntu 20.04 na wirutalnej maszynie.
1. Stworzyłem virtualny port can0, za pomocą vcan.
	sudo modprobe vcan 
	sudo ip link add dev can0 type vcan 
	sudo ip link set dev can0 up

2. Sprawdziłem czy port się stworzył i jest w uruchomiony (up)
	ip a

3. Następnie uruchomiłem 6 terminali, na których testowałem ROS'a i moją paczkę.
   (Dwa terminale do sprawdzenia wyników, jeden roscore i 3 uruchomione paczki)
	roscore
	rosrun socketcan_bridge topic_to_socketcan_node
	candump can0
	rostopic echo /sent_messages
	rosrun lazik Converter_node
	rosrun lazik Generator_node
	
4. Candump - pokazuje format wysłanych ramek CAN.
   Rostopic echo /sent_messages - pokazuje jakie dane są wysłane przez topic.


Moim zdaniem program realizuje jego zadanie, o ile czegoś źle nie zrozumiałem z waszego opisu.
PS.Za ewentuualne błędny, niedociągnięcia przepraszam, to była moja pierwsza styczność z ROS'em

