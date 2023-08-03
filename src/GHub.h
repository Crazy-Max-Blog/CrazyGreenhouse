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
#include <GyverHub.h>
#include <Stamp.h>
//#include "data.h"

GyverHub hub;//("CrazyDevices", "greenhouse", "");
// иконки
// https://fontawesome.com/v5/cheatsheet/free/solid
// https://fontawesome.com/v5/search?o=r&m=free&s=solid

void hub_tick() {
  // тикер. Вызывать в loop
  hub.tick();

  static GHtimer tmr(3000);
  if (tmr.ready()) {                                  // таймер на 3 секунды
    // hub.sendUpdate("ga1", String(random(-5, 30)));
    // if (hub.focused()) Serial.println("focus");  // проверяем, смотрит ли кто на нас
    // hub.print(String("hello #") + millis());     // печатать в веб-консоль
    // hub.sendPush("Hello!");                      // отправить пуш-уведомление

    // hub.sendUpdate("lbl,text");                  // отправить апдейт, получить значения из build
    // hub.sendUpdate("lbl", String(random(100)));  // отправить апдейт

    // hub.sendGet("lbl,text");                     // отправить значения на get топик, получить значения из build
    // hub.sendGet("lbl", String(random(100)));     // отправить значение на get топик
  }
}

void tab1() {
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
    hub.Title(F("Настройки автоматизаций"));

    int xt = 0;
    Serial.println(xt);
    for(int i=sizeof(gtime)/8;i!=0;i--) {
      Serial.println("1");
      Serial.println(i);
      Serial.println(sizeof(gtime)/8);
      Serial.println(ti(i).timeToString());
      if(ti(i).timeToString()!=tnull.timeToString()) break;
      xt=i-1;
      Serial.println("");
    }
    Serial.println(xt);
      
    for(int i=0;i<=xt;i++) {
        String ttt = "Время " + String(i+1) + " полива";
        hub.Time(&ti(i), ttt, GH_YELLOW);
    }

    if(hub.Button(0, "Add...")) {
      Serial.println("Add...");
      Serial.println(xt);
      if(ti(sizeof(gtime)/8).timeToString() != tnull.timeToString()) {
        ti(xt).set(18, 0, 0);
      }
    }
    
    hub.Spinner(&spin_f, GH_FLOAT, F("Размер порции полива"), 0, 100, 0.5);
    
    hub.Title(F("Настройки WiFi"));
    
    hub.Time(&gtime1, F("Время первой кормёшки"), GH_YELLOW);
    hub.Time(&gtime2, F("Время второй кормёшки"), GH_YELLOW);
    hub.Time(&gtime3, F("Время третьей кормёшки"), GH_YELLOW);
    hub.Spinner(&spin_f, GH_FLOAT, F("Размер порции еды"), 0, 100, 0.5);
    hub.Spinner(&spin_f, GH_FLOAT, F("Размер порции воды"), 0, 100, 0.5);
}

void hub_build() {
  // hub.buildRead();  // билдер вызван для set или read операций

  GHbuild b = hub.getBuild();  // получить свойства текущего билда

  // Serial.println(GHreadBuild(b.type));    // тип билда
  // Serial.println(GHreadConn(b.hub.conn)); // соединение
  // Serial.println(b.hub.id);               // id клиента
  if (b.type == GH_BUILD_ACTION) {                  // это действие
    Serial.println(b.action.name);                // имя компонента
    Serial.println(b.action.value);               // значение
    Serial.println();
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(WiFi.localIP());
  
  hub.setupMQTT("test.mosquitto.org", 1883);
#endif

  // обработчик билда
  hub.onBuild(hub_build);

  // обработчик текста из веб-консоли
  hub.onCLI([](String & s) {
    Serial.println(s);
    hub.print(s);   // отправить эхо
    // hub.print(s, GH_RED);
  });

  // обработчик статуса для отладки
  hub.onEvent([](GHevent_t event, GHconn_t conn) {
    // GHreadXXXX берёт текстовое описание статуса из PROGMEM
    Serial.print(GHreadConn(conn));
    Serial.print(": ");
    Serial.println(GHreadEvent(event));
  });

  // вывести причину перезагрузки
  hub.onReboot([](GHreason_t r) {
    Serial.println(GHreadReason(r));
  });

  // добавить поля в Info
  hub.onInfo([](GHinfo_t info) {
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
  });

  hub.begin();    // запустить
  // hub.setVersion("v1.1");      // установить версию (отображается в info)
  // hub.setPIN(1234);            // установить пин-код
  // hub.sendGetAuto(true);       //автоматически отправлять новое состояние на get-топик

  dlog.begin();           // лог тоже запустить
}
