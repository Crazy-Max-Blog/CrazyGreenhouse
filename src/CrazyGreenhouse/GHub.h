// ====== НАСТРОЙКИ КОМПИЛЯЦИИ ======
//#define ATOMIC_FS_UPDATE  // OTA обновление GZIP файлом
#define GH_ASYNC          // использовать ASYNC библиотеки

// включить сайт в память программы (не нужно загружать файлы в память)
#define GH_INCLUDE_PORTAL

// отключение модулей
//#define GH_NO_PORTAL    // открытие сайта из памяти esp
//#define GH_NO_WS        // WebSocket
//#define GH_NO_MQTT      // MQTT
//#define GH_NO_FS        // работа с файлами (включая ОТА!)
//#define GH_NO_OTA       // ОТА файлом с приложения
//#define GH_NO_OTA_URL   // ОТА по URL
// =================================

#include <Arduino.h>
#define GH_INCLUDE_PORTAL
#include <GyverHub.h>
#include <Stamp.h>
//#include "data.h"

GyverHub hub;//("CrazyDevices", "greenhouse", "");
// иконки
// https://fontawesome.com/v5/cheatsheet/free/solid
// https://fontawesome.com/v5/search?o=r&m=free&s=solid

void log(String text = "", bool n = true) {
  if(n) Serial.println(text);
  else Serial.print(text);
  hub.print(text, GH_RED);
}

void hub_tick() {
  // тикер. Вызывать в loop
  hub.tick();

  if(reset.changed()) {
    //SAVE_DATA;
    RESET;
  }

  static GHtimer tmr(3000);
  if (tmr.ready()) {                                  // таймер на 3 секунды
    // hub.sendUpdate("ga1", String(random(-5, 30)));
    // if (hub.focused()) log("focus");  // проверяем, смотрит ли кто на нас
    // hub.print(String("hello #") + millis());     // печатать в веб-консоль
    // hub.sendPush("Hello!");                      // отправить пуш-уведомление

    // hub.sendUpdate("lbl,text");                  // отправить апдейт, получить значения из build
    // hub.sendUpdate("lbl", String(random(100)));  // отправить апдейт

    // hub.sendGet("lbl,text");                     // отправить значения на get топик, получить значения из build
    // hub.sendGet("lbl", String(random(100)));     // отправить значение на get топик
  }
}

void tab1() {
    hub.Title(F("Главная"));
  
    hub.Title(F("Датчики"));
    
    //hub.Label("a", "Температура: " + String(t) + " °С", "", GH_DEFAULT, 20);
    //hub.Label("b", "Влажность: " + String(h) + " %", "", GH_DEFAULT, 20);
    //hub.Label("c", "Уровень: " + String(u) + " %", "", GH_DEFAULT, 20);
    hub.WidgetSize(33);
    hub.Gauge(t, F("°С"), F("Температура"), 0, 50, 0.5, GH_DEFAULT);
    hub.Gauge(h, F("%"), F("Влажность"), 0, 100, 0.5, GH_DEFAULT);
    hub.Gauge(u, F("%"), F("Уровень"), 0, 100, 0.5, GH_DEFAULT);
    hub.WidgetSize(100);


    
    hub.Title(F("Управление"));

    hub.WidgetSize(50);
    hub.Switch(&w1, F("1 окно"), GH_DEFAULT);
    hub.Switch(&w2, F("2 окно"), GH_RED);
    hub.WidgetSize(100);
    hub.Button(&p, F("Полить"), GH_RED);
}

void tab2() {
  hub.Title(F("Видео"));
  
  hub.Stream();
}

void tab3() {
  hub.Title(F("Настройки"));
  
  hub.Title(F("Настройки работы"));
    
  hub.Spinner(&porpol, GH_FLOAT, F("Размер порции полива"), 0, 100, 0.5);
    
  hub.Title("Настройки WiFi");

  bool upd = false;
  upd |= hub.Input(&db.wssid, GH_STR, F("Логин"));
  upd |= hub.Pass(&db.wpass, GH_STR, F("Пароль"));
  if (upd) memory.update();
  hub.Button(&reset, "Сохранить и перезагрузить");    
}

void hub_build() {
  memory.update();
  // hub.buildRead();  // билдер вызван для set или read операций

  GHbuild b = hub.getBuild();  // получить свойства текущего билда

  // log(GHreadBuild(b.type));    // тип билда
  // log(GHreadConn(b.hub.conn)); // соединение
  // log(b.hub.id);               // id клиента
  if (b.type == GH_BUILD_ACTION) {                  // это действие
    log(b.action.name);                // имя компонента
    log(b.action.value);               // значение
    log(db.wssid);
    /*switch (b.action.name) {
      case 'wssid': db.wssid = b.action.value; break;
      case 'wpass': db.wpass = b.action.value; break;
      default: break;
    }*/

    String name = b.action.name;

    if(name == "wssid") db.wssid = b.action.value;
    else if(name == "wpass") db.wpass = b.action.value;
    
    log();
  }

  
  //hub.Tabs(&tab, F("Tab 1,MY TAB,tab 2,tab 3,TAB 4"));
  hub.Menu(F("Главная,Видео,Настройки"));

  switch (hub.menu) {
    case 0: tab1();break;
    case 1: tab2();break;
    case 2: tab3();break;
    default: break;
  }
}


void hub_start(String l, String p, char* pr, char* n, char* i) {
  hub.config(pr, n, i);
  hub.setupStream(&Serial, GH_SERIAL);
#ifdef GH_ESP_BUILD
  WiFi.mode(WIFI_STA);
  WiFi.begin(l, p);
  for(int i = 0;i<=30;i++) {
    delay(500);
    log(".", false);
  }
  
  log();
  
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP("WebLamp " + WiFi.softAPIP().toString(), "12345678");
  } else {
    log(WiFi.localIP().toString());
    hub.setupMQTT("test.mosquitto.org", 1883);
  }
#endif

  // обработчик билда
  hub.onBuild(hub_build);

  // обработчик текста из веб-консоли
  hub.onCLI([](String & s) {
    log(s);
    hub.print(s, GH_RED);   // отправить эхо
    // hub.print(s, GH_RED);
  });

  // обработчик статуса для отладки
  hub.onEvent([](GHevent_t event, GHconn_t conn) {
    // GHreadXXXX берёт текстовое описание статуса из PROGMEM
    log(GHreadConn(conn));
    log(": ");
    log(GHreadEvent(event));
  });

  // вывести причину перезагрузки
  hub.onReboot([](GHreason_t r) {
    log(GHreadReason(r));
  });

  // добавить поля в Info
  /*hub.onInfo([](GHinfo_t info) {
    switch (info) {
      case GH_INFO_VERSION:
        hub.addInfo(F("Custom_ver"), F("v1.5"));
        break;
      case GH_INFO_NETWORK:
        hub.addInfo(F("Custom_net"), "net value");
        break;
      case GH_INFO_MEMORY:
        hub.addInfo(F("Custom_mem"), String(123));
        break;
      case GH_INFO_SYSTEM:
        hub.addInfo(F("Custom_sys"), "text");
        break;
    }
  });*/

  hub.begin();    // запустить
  hub.setVersion("Crazy-Max-Blog/CrazyGreenhouse@v0.1");      // установить версию (отображается в info)
  // hub.setPIN(1234);            // установить пин-код
  // hub.sendGetAuto(true);       //автоматически отправлять новое состояние на get-топик

  //dlog.begin();           // лог тоже запустить
}