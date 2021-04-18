#define _CRT_SECURE_NO_WARNINGS
#include <bangtal.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <Sound.h>
#include<windows.h>



SceneID screen;
ObjectID title,start, back_screen;
ObjectID bullet[6];
ObjectID empty_bullet[6];
ObjectID monster[3];
ObjectID my_heart[3];
ObjectID gun_fire;
ObjectID end;
ObjectID restart;
ObjectID easy, normal, hard;
SoundID start_music, gunfire_sound, reload_sound, lost_hp, ending_music, beep;


TimerID monster_timer[3];


int gun_timer_val;
int reload_timer_val;

//총알 쏜 횟수
int cnt_bullet;
//총알 좌표
int x_bullet[6] = { 0,50,100,150,200,250 };
int x_heart[3] = { 550, 630, 710 };
//좌표
int monster_x = 0;
int monster_y = 0;

//몹들의 id
int monster_id;

//몹들의 체력
int monster_hp[3];

//점수
int score;

//체력
int my_hp;

//난이도
int difficult;

//엔딩멘트
char ending_ment[100];

ObjectID createObject(const char* imgae, SceneID scene, int x, int y, bool shown = true) {

    ObjectID object = createObject(imgae);
    locateObject(object, scene, x, y);
    
    if (shown == true) {
        showObject(object);
    }

    return object;
}

//게임 초기설정
SceneID game_init() {
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

    //800*800 
    screen = createScene("main","background.png");

    //400*100
    title = createObject("title.png", screen, 200, 500);
    //200*50
    start = createObject("start.png", screen, 300, 200);
    back_screen = createObject("background_object.png", screen, 0, 0, false);

    monster[0] = createObject("monster1.png", screen, 0, 0, false);
    monster[1] = createObject("monster1.png", screen, 0, 0, false);
    monster[2] = createObject("monster1.png", screen, 0, 0, false);

    end = createObject("end.png", screen, 200, 500, false);
    restart = createObject("restart.png", screen, 300, 400, false);

    gun_fire = createObject("gun_fire.png", screen, 0, 0, false);

    easy = createObject("easy.png", screen, 150, 100);
    normal = createObject("normal.png", screen, 350, 100);
    hard = createObject("hard.png", screen, 550, 100);


    start_music = createSound("start_music.mp3");
    playSound(start_music, true);

    lost_hp = createSound("lost_hp.mp3");

    ending_music = createSound("gameover.mp3");

    beep = createSound("beep.mp3");

    gunfire_sound = createSound("fire.mp3");
    reload_sound = createSound("reload.mp3");

    //기본 난이도
    difficult = 4;
    return screen;

}




void game_end() {
    showObject(end);
    showObject(restart);
    playSound(ending_music, true);

    sprintf(ending_ment, "You caught %d monster.", score);
    showMessage(ending_ment);

    locateObject(easy, screen, 150, 201);
    locateObject(normal, screen, 350, 201);
    locateObject(hard, screen, 550, 201);

    showObject(easy);
    showObject(normal);
    showObject(hard);
}



void set_bullet_img() {
    //탄이 빠짐
    for (int i = 0; i < 6; i++) {
        empty_bullet[i] = createObject("empty_bullet.png", screen, x_bullet[i] + 10, 50, true);
    }

    //탄이 들어가 있음
    for (int i = 0; i < 6; i++) {
        bullet[i] = createObject("bullet.png", screen, x_bullet[i]+10, 50, true);
    }
}

void set_my_heart() {
    for (int i = 0; i < 3; i++) {
        my_heart[i] = createObject("my_empty_hp.png", screen, x_heart[i], 100, true);

    }

    for (int i = 0; i < 3; i++) {
        my_heart[i] = createObject("my_hp.png", screen, x_heart[i], 100, true);
        
    }
}

//50, 250 ~ 650, 650
void respawn_moster(ObjectID object) {

    monster_x = rand() % (650 + 1 - 50) + 50;
    monster_y = rand() % (650 + 1 - 250) + 250;

    locateObject(object, screen, monster_x, monster_y);
    showObject(object);
}

void game_start() {
    
    //총알 쏜 횟수
    cnt_bullet = 0;

    hideObject(start);
    hideObject(title);
    set_bullet_img();
    set_my_heart();

    //몬스터 위치 설정
    respawn_moster(monster[0]);
    respawn_moster(monster[1]);
    respawn_moster(monster[2]);

    //몬스터 체력 초기화
    monster_hp[0] = 2;
    monster_hp[1] = 2;
    monster_hp[2] = 2;

    //몬스터 타이머 생성 밎 시작
    monster_timer[0] = createTimer(difficult);
    monster_timer[1] = createTimer(difficult);
    monster_timer[2] = createTimer(difficult);
    startTimer(monster_timer[0]);
    startTimer(monster_timer[1]);
    startTimer(monster_timer[2]);




    //점수 초기화 , 체력 초기화
    score = 0;
    my_hp = 3;
    stopSound(start_music);

    hideObject(easy);
    hideObject(normal);
    hideObject(hard);
}

//재장전
void reload() {
    playSound(reload_sound);
    for (int i = 0; i < 6; i++) {
        showObject(bullet[i]);
    }

    cnt_bullet = 0;

    Sleep(1000);
}

//총 발사
void shoot_gun(int x, int y) {
    locateObject(gun_fire, screen, x - 15, y - 15);
    showObject(gun_fire);
    playSound(gunfire_sound);

}

//총알 수 체크
void check_bullet_cnt (int cnt) {
    switch (cnt)
    {
    case 0:
        hideObject(bullet[0]);
        cnt_bullet++;
        break;

    case 1:
        hideObject(bullet[1]);
        cnt_bullet++;
        break;

    case 2:
        hideObject(bullet[2]);
        cnt_bullet++;
        break;

    case 3:
        hideObject(bullet[3]);
        cnt_bullet++;
        break;

    case 4:
        hideObject(bullet[4]);
        cnt_bullet++;
        break;

    case 5:
        hideObject(bullet[5]);
        cnt_bullet++;
        break;

    case 6:
        reload();
        break;
    }
}

//몬스터 id 체크
int check_monster(ObjectID object) {
    if (object == monster[0]) {
        return 0;
    }

    else if (object == monster[1]) {
        return 1;
    }

    else if (object == monster[2]) {
        return 2;
    }
}

//callback 함수들
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {

    if (object == back_screen || object == monster[0] || object == monster[1] || object == monster[2]) {
        shoot_gun(x, y);
        check_bullet_cnt(cnt_bullet);

        if (object == monster[0] || object == monster[1] || object == monster[2]) {
            monster_id = check_monster(object);
            //클릭을 하면 hp 1 감소
            monster_hp[monster_id] = monster_hp[check_monster(object)] - 1;
            
            //0이면 다시 스폰 score 증가
            if (monster_hp[monster_id] == 0) {
                respawn_moster(object);
                monster_hp[monster_id] = 2;
                setTimer(monster_timer[monster_id], difficult);
                score++;
            }

        }

    }

    if (object == start) {
        playSound(beep);
        showObject(back_screen);
        game_start();
    }

    if (object == restart) {
        playSound(beep);
        hideObject(end);
        hideObject(restart);

        game_start();
        stopSound(ending_music);
    }

    if (object == easy) {
        playSound(beep);
        difficult = 5;
    }

    if (object == normal) {
        playSound(beep);
        difficult = 4;
    }

    if (object == hard) {
        playSound(beep);
        difficult = 3;
    }
 
}

void monster_time_reset() {
    setTimer(monster_timer[0], difficult);
    setTimer(monster_timer[1], difficult);
    setTimer(monster_timer[2], difficult);
}


void timerCallback(TimerID timer) {
    if (my_hp == 3) {
        my_hp--;
        hideObject(my_heart[2]);
        playSound(lost_hp);
        monster_time_reset();
        
    }
    else if (my_hp == 2) {
        my_hp--;
        hideObject(my_heart[1]);
        playSound(lost_hp);

        monster_time_reset();
    }

    else if (my_hp == 1) {
        hideObject(my_heart[0]);
        playSound(lost_hp);
        game_end();
        monster_time_reset();
    }
}


int main()
{
    srand(time(NULL));

    setMouseCallback(mouseCallback);
    setTimerCallback(timerCallback);
    startGame(game_init());


}

