#include <bangtal.h>
using namespace bangtal;


int main()
{
	//장면생성
	auto scene1 = Scene::create("room1", "images/배경-1.png");
	auto scene2 = Scene::create("room2", "images/배경-2.png");
	auto scene3 = Scene::create("room3", "images/배경-3.png");

	//열쇠생성
	auto key = Object::create("images/열쇠.png", scene1, 600, 150);
	key->setScale(0.2f);

	key->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		key->pick();
		return true;

		});

	//화분생성(열쇠를 먼저 만들고 화분을 만든다)
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

	//문 생성
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

	//키패드 생성
	auto keypad = Object::create("images/키패드.png", scene2, 885, 420);
	keypad->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showKeypad("BANGTAL", door3); //키패드 암호, 이벤트 수신할 오브젝트 설정
		return true;
	});

	auto password = Object::create("images/암호.png", scene2, 400, 100, false);

	
	//스위치 생성
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


	auto timer = Timer::create(100.f);
	showTimer(timer);
	timer->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("탈출 실패!");
		endGame();
		return true;

	});
	
	timer->start();

	//게임시작
	startGame(scene1);

	return 0;
}