# RoomEscape

## 게임 코드 소개

```c++
#include <bangtal.h>
using namespace bangtal;


int main()
{
	auto scene1 = Scene::create("room1", "images/배경-1.png");
	auto scene2 = Scene::create("room2", "images/배경-2.png");
	auto scene3 = Scene::create("room3", "images/배경-3.png");
```
방탈출 게임 장면 생성

```c++
	auto key = Object::create("images/열쇠.png", scene1, 600, 150);
	key->setScale(0.2f);

	key->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		key->pick();
		return true;

		});

	auto flowerpot_moved = false;
	auto flowerpot = Object::create("images/화분.png", scene1, 550, 150);
	flowerpot->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (flowerpot_moved == false){
			if (action == MouseAction::MOUSE_DRAG_LEFT) {

				flowerpot->locate(scene1, 450, 150);
				flowerpot_moved = true;
			}
			else if (action == MouseAction::MOUSE_DRAG_RIGHT) {

				flowerpot->locate(scene1, 650, 150);
				flowerpot_moved = true;
			}
		}

		return true;
	});
```
열쇠와 화분생성(열쇠가 화분 뒤에 위치해야 하므로 열쇠를 먼저 만들고 화분을 생성한다. 

```c++
	auto open1 = false;
	auto door1 = Object::create("images/문-오른쪽-닫힘.png", scene1, 800, 270);


	door1->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
		if (open1 == true) {
			scene2->enter();
		}
		else if(key->isHanded()){
			door1->setImage("images/문-오른쪽-열림.png");
			open1 = true;
		}
		else {
			showMessage("열쇠가 필요해!");
		}
		return true;

	});
```
첫번째 방 장면에서 문 생성, 열쇠를 습득하면 문을 열 수 있도록 설정

```c++
	auto door2 = Object::create("images/문-왼쪽-열림.png", scene2, 320, 270);
	door2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene1->enter();
		return true;
	});

	auto open3 = false, locked3 = true;
	auto door3 = Object::create("images/문-오른쪽-닫힘.png", scene2, 900, 270);
	door3->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (locked3) {
			showMessage("문이 잠겨있다");
		}
		else if (open3 == true) {
			scene3->enter();
		}
		else {
			door3->setImage("images/문-오른쪽-열림.png");
			open3 = true;
		}
	
		return true;

	});

	door3->setOnKeypadCallback([&](ObjectPtr object)->bool {
		locked3 = false;
		showMessage("Open");
		return true;
	});
  ```
  두번째 방에서 두가지 문 생성, door2는 첫번째 방으로 돌아갈 수 있는 문, door3는 세번째 방으로 넘어갈 수 있는 문으로 설정, 키패드에 암호 입력해야 들어갈 수 있음.
  
```c++
	auto keypad = Object::create("images/키패드.png", scene2, 885, 420);
	keypad->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showKeypad("BANGTAL", door3); //키패드 암호, 이벤트 수신할 오브젝트 설정
		return true;
	});

	auto password = Object::create("images/암호.png", scene2, 400, 100, false);
```
키패드 생성, 암호 BANGTAL로 설정함

```c++
	auto lighted = true;
	auto button = Object::create("images/스위치.png", scene2, 880, 449);
	button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (lighted) {                //켜진상태
			scene2->setLight(0.2f);
			password->show();
			lighted = false;
		}
		else {                        //꺼진상태
			scene2->setLight(1.0f);
			password->hide();
			lighted = true;
		}

	    return true;
	});
```
스위치 생성, 스위치를 끄면 암호 힌트가 나옴

```c++

	auto open4 = false, locked4 = true; 
	auto door4 = Object::create("images/문-오른쪽-닫힘.png", scene3, 800, 270);
	door4->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (locked4) {
			showMessage("열쇠가 있던 곳은 어디였을까?");
		}
		else if (open4 == true) {
			endGame();
		}
		else {
			door4->setImage("images/문-오른쪽-열림.png");
			open4 = true;
		}

		return true;

		});

	door4->setOnKeypadCallback([&](ObjectPtr object)->bool {
		locked4 = false;
		showMessage("탈출 성공!");
		return true;
		});

	auto keypad2 = Object::create("images/키패드.png", scene3, 785, 420);
	keypad2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showKeypad("PLANT", door4); 
		return true;
		});

```
door4 생성, 클릭하면 암호에 대한 힌트 출력. 키패드에 암호 입력하면 탈출 가능

```c++

	auto timer = Timer::create(100.f);
	showTimer(timer);
	timer->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("탈출 실패!");
		endGame();
		return true;

	});
	
	timer->start();
  
  ```
  방탈출 게임 시간제한 100초로 설정, 시간이 끝나면 게임 종료
  ```c++

	//게임시작
	startGame(scene1);

	return 0;
}
```
