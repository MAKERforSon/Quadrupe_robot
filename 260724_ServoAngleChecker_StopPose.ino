#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// --- サーボ換算用パラメータ ---
const float DEG_TO_PCA = 2.27; // 1度あたりのPCAカウント変化量
const int   PCA_CENTER = 325;  // 90度（付け根のセンター）のPCAカウント値

// 各ひざサーボの初期角度（接地時）
const float FR_KNEE_INIT = 180.0; // 1番ピン（前右）
const float FL_KNEE_INIT = 0.0;   // 3番ピン（前左）
const float BR_KNEE_INIT = 0.0;   // 5番ピン（後右）
const float BL_KNEE_INIT = 180.0; // 7番ピン（後左）

// --- 歩行モーションのパラメータ ---
const float PERIOD = 1500.0;     // 周期
const float SWING_AMP = 20.0;    // 付け根の振幅
const float KNEE_LIFT = 20.0;    // 足を持ち上げる高さ（いらない場合は0.0に）

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n--- 8-Servo: Walking Mode ---");

  Wire.begin(21, 22); 
  pwm.begin();
  pwm.setPWMFreq(50); 

  // 🛠️ 起動直後の姿勢でスタンバイ
  moveServo(0, 0.0, 90.0);           // 0番：前右・付け根
  moveServo(1, 0.0, FR_KNEE_INIT);   // 1番：前右・ひざ
  
  moveServo(2, 0.0, 90.0);           // 2番：前左・付け根
  moveServo(3, 0.0, FL_KNEE_INIT);   // 3番：前左・ひざ
  
  moveServo(4, 0.0, 90.0);           // 4番：後右・付け根
  moveServo(5, 0.0, BR_KNEE_INIT);   // 5番：後右・ひざ
  
  moveServo(6, 0.0, 90.0);           // 6番：後左・付け根
  moveServo(7, 0.0, BL_KNEE_INIT);   // 7番：後左・ひざ
  
  delay(1000);
}

void loop() {
}

/**
 * サーボを任意の角度に動かす関数
 */
void moveServo(int ch, float offset_deg, float base_deg) {
  float total_deg = base_deg + offset_deg;
  int pca_value = PCA_CENTER + (int)((total_deg - 90.0) * DEG_TO_PCA);
  
  if (pca_value < 150) pca_value = 150;
  if (pca_value > 500) pca_value = 500;

  pwm.setPWM(ch, 0, pca_value);
}
