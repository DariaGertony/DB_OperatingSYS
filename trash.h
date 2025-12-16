//
// Created by Admin on 23.11.2025.
//

#ifndef SCRIPT_PY_TRASH_H
#define SCRIPT_PY_TRASH_H


inline QString encodeInt(int value) {
    string str;
    // Правильное преобразование с учетом знака
    str += (char)((value >> 24) & 0xFF);
    str += (char)((value >> 16) & 0xFF);
    str += (char)((value >> 8) & 0xFF);
    str += (char)(value & 0xFF);
    return QString::fromStdString(str);
}

// Декодирование int
inline int decodeint(QString symb) {
    string str = symb.toStdString();
    if (str.length() < 4) return 0;
    int id = ((str[0] * 256 + str[1])*256 + str[2]) * 256 + str[3];


    return id;
}
inline QString encodeFloat(float value) {
    value*=100;
    int res = (int)(value);
    return encodeInt(res);
}

inline float decodeFloat(QString symb) {
    return (float)(decodeint(symb))/(100.0);
}


inline QString encodeUnsignedInt(unsigned int value) {
    string str;
    str += (char)((value >> 24) & 0xFF);
    str += (char)((value >> 16) & 0xFF);
    str += (char)((value >> 8) & 0xFF);
    str += (char)(value & 0xFF);
    return QString::fromStdString(str);
}

// Декодирование unsigned int
inline unsigned int decodeid(QString symb) {
    string str = symb.toStdString();
    if (str.length() < 4) return 0;

    // Для беззнакового - все байты беззнаковые
    return ((uint8_t)str[0] << 24) |
           ((uint8_t)str[1] << 16) |
           ((uint8_t)str[2] << 8) |
           (uint8_t)str[3];
}

#endif //SCRIPT_PY_TRASH_H