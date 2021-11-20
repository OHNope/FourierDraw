//
// Created by XBY on 2021/11/19.
//

#ifndef FOURIERDRAW_FFT_HPP
#define FOURIERDRAW_FFT_HPP

const int maxn = 3e6 + 5;
const double pi = acos(-1);
int rev[maxn];
typedef std::complex<double> cp;

void FFT(int n, cp *t, int mark) {
    for (int i = 0; i < n; ++i)
        if (i < rev[i])
            swap(t[i], t[rev[i]]);    // 将每个数放在它对应的位置上
    for (int i = 1; i < n; i <<= 1) { // 枚举每段的长度
        cp wn(cos(pi / i), mark * sin(pi / i));
        for (int j = 0; j < n; j += 2 * i) {              // 枚举每个段组
            cp w = 1;                                     // wn^0
            for (int k = j; k < j + i; k++, w = w * wn) { // 枚举计算
                cp term1 = t[k], term2 = t[k + i] * w;
                t[k] = term1 + term2;
                t[k + i] = term1 - term2;
            }
        }
    }
    if (mark ^ 1)
        for (int i = 0; i < n; i++)
            t[i] /= n;
}

#endif //FOURIERDRAW_FFT_HPP
