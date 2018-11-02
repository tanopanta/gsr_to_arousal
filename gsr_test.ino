// GSRセンサーで感情の縦軸をとる

// センサーの2つの電極に指を入れていない状態＝抵抗マックスのときに、およそ2047(入力電圧のの半分)になる、ならない場合はねじを回せばよいらしい。

// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC2877802/
// によると自律神経性覚醒はSCR(GSR)の Area under the curve(AUC)を指標として使える。
//
// サンプルコードは
// scr_sf_auc(scr):
//     scr = scr - min(scr)
//     auc = mean(scr)
//     return auc

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
    data = min(data, MAX_GSR); //センサーの最大値を超えたら最大値とする

    //Serial.println(data);
    gsrs[pointer++] = data;

    if(pointer % 600 == 0) {
        int n = pointer;
        //最小値
        int min_gsr = MAX_GSR;
        for(int i = 0; i < n; i++) {
            min_gsr = min(gsrs[i], min_gsr);
        }

        int sum = 0;
        for(int i = 0; i < n; i++) {
            sum += gsrs[i] - min_gsr;
        }
        Serial.print("min_gsr=");
        Serial.println(min_gsr);
        double auc = (double)sum / n;

        auc = (2048-auc)/((4096+2*auc)*10000);//校正
        auc = auc * 1000000;//単位をuSに
        Serial.print("auc=");
        Serial.print(auc);
        Serial.println("uS");
        pointer = 0;
    }
    
    delay(100);
}
