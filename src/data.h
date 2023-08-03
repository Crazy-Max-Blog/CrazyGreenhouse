// WiFi
#define AP_SSID "MiHome"
#define AP_PASS "!vlad-02072015!"

uint8_t tab = 0;
GHbutton b1, b2;

bool w1, w2;
GHbutton p;

String label_s;

String inp_str;
char inp_cstr[20];
int16_t inp_int;
float inp_float;
String pass;

int16_t sld = 20;
float sld_f = 20;

int16_t spin;
float spin_f;

bool sw;
GHcolor col;

Stamp tnull(12, 30, 1);

Stamp gtime[6] = {tnull, tnull, tnull, tnull, tnull, tnull};

Stamp t1(12, 30, 1), t2(12, 30, 1), t3(12, 30, 1), t4(12, 30, 1), t5(12, 30, 1), t6(12, 30, 1), nn(1, 2, 3);

Stamp ti(int num) {
  switch (num) {
    case 1: return t1;
    case 2: return t2;
    case 3: return t3;
    case 4: return t4;
    case 5: return t5;
    case 6: return t6;
    default: return nn;
  }
}

Stamp gdate(2023, 3, 10);
Stamp gtime1(12, 30, 0);
Stamp gtime2(12, 30, 0);
Stamp gtime3(12, 30, 0);
Stamp gdatetime(2025, 6, 20, 06, 06, 06);

uint8_t sel;
GHflags flags;

String disp_s;
String html_s;
GHlog dlog;
GHpos pos;

float t = 28.52;
float h = 42.49;
float u = 70;
