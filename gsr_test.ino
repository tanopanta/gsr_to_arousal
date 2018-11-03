// GSRセンサーで感情の縦軸をとる

// 皮膚コンダクタンスは自律神経系の覚醒の指標として使用できる
// 根拠
// Emotion and motivation I: defensive and appetitive reactions in picture processing.
//https://www.researchgate.net/publication/10600214_Emotion_and_Motivation_I_Defensive_and_Appetitive_Reactions_in_Picture_Processing
// 被引用数1924

// センサーの2つの電極に指を入れていない状態＝抵抗マックスのときに、およそ2047(入力電圧のの半分)になる、ならない場合はねじを回せばよいらしい。

//　https://www.birmingham.ac.uk/Documents/college-les/psych/saal/guide-electrodermal-activity.pdf
//　によるとサンプリング周波数は1-5 samples per secondで良い。ただし感情の想起のタイミングが重要な場合は1msec


#include <M5Stack.h>

#include <math.h>


const int PIN_GSR = 35;
const int MAX_GSR = 2047;
int gsrs[1024];
int pointer = 0;
void setup() {
    M5.begin();
    dacWrite(25, 0); // Speaker OFF
    Serial.begin(115200);
}



void loop() {
    int data = analogRead(PIN_GSR);
    data = min(data, MAX_GSR); //センサーの最大値を超えたら最大値に固定

    //Serial.println(data);
    gsrs[pointer++] = data;

    if(pointer % 10 == 0) {
        int n = pointer;

        long sum = 0;
        for(int i = 0; i < n; i++) {
            sum += gsrs[i];
        }
        
        double avg_gsr = (double)sum / n;

        avg_gsr = (2048-avg_gsr)/((4096+2*avg_gsr)*10000);//校正
        avg_gsr = avg_gsr * 1000000;//単位をuSに
        Serial.print("gsr=");
        Serial.print(avg_gsr);
        Serial.println("uS");
        Serial.print("arousal=");
        Serial.println(avg_gsr / 50 * 8 + 1);//1-9にノーマライズ
        pointer = 0;
    }
    
    delay(100);
}
