//
// Created by Admin on 30.10.2025.
//
#pragma once

#ifndef DBSYS_CODERS_H
#define DBSYS_CODERS_H
#include <qstring.h>
using namespace std;
#include <iostream>



inline QByteArray encodeInt(int value) {
    QByteArray str;
    str.reserve(4);
    str += static_cast<char>(static_cast<unsigned char>((value >> 24) & 0xFF));
    str += static_cast<char>(static_cast<unsigned char>((value >> 16) & 0xFF));
    str += static_cast<char>(static_cast<unsigned char>((value >>  8) & 0xFF));
    str += static_cast<char>(static_cast<unsigned char>((value      ) & 0xFF));
    return str;
}

inline int decodeInt(QByteArray symb) {
    std::string str = symb.toStdString();
    if (str.length() < 4) return 0;

    uint32_t u = (static_cast<uint8_t>(str[0]) << 24) |
                 (static_cast<uint8_t>(str[1]) << 16) |
                 (static_cast<uint8_t>(str[2]) <<  8) |
                 (static_cast<uint8_t>(str[3])      );

    return static_cast<int>(u);
}

inline QByteArray encodeUnsignedInt(unsigned int value) {
    return encodeInt(static_cast<int>(value));

}

inline unsigned int decodeUnsignedInt(QByteArray symb) {
    std::string str = symb.toStdString();
    if (str.length() < 4) return 0;

    return (static_cast<uint8_t>(str[0]) << 24) |
           (static_cast<uint8_t>(str[1]) << 16) |
           (static_cast<uint8_t>(str[2]) <<  8) |
           (static_cast<uint8_t>(str[3])      );
}

inline QByteArray encodeFloat(float value) {
    int u;
    std::memcpy(&u, &value, sizeof(value));
    return encodeUnsignedInt(u);
}
inline float decodeFloat(QByteArray symb) {
    int u = decodeInt(symb);
    float f;
    std::memcpy(&f, &u, sizeof(f));
    return f;
}

inline QByteArray encodeBool(bool value) {
    QByteArray str;
    str += (char)(value ? 1 : 0);

    return str;
}




inline QString decodebool(QString symb) {
    if (symb[0] == char(0)) {
        return "false";
    }
    else {
        return "true";
    }
}


inline int StringHash(const QString &str, int size) {
    int hash = 0;
    for (int i = 0; i < str.length(); ++i) {
        hash = (hash * 31 + str[i].unicode())%size;
    }
    return hash;
}


inline vector<unsigned int> findvalue(QString pathfile, QByteArray value, QString type, int sizet, QString bdpath, int strclen, int infolen, int strcpos, int ifkey) {
    QFile file(pathfile);
    QFile bd(bdpath);
    int size = sizet*8;
    unsigned int id = 0;
    vector<unsigned int> res;
    if (file.open(QFile::ReadWrite) and bd.open(QFile::ReadWrite)) {
        int pos=0;
        int stpos=pos;
        QByteArray f;

        if (type == "int") {
            cout<<222;
            int val = decodeInt(value);
            pos = (val*4)%size;
            stpos = pos;
            int fl =0;
            while (true) {
                file.seek(pos);
                QByteArray bytes(4,'\0');
                f = file.read(4);
                if(f==bytes or (pos == stpos and fl ==1)) {
                    res.push_back(0);
                    return res;
                }
                pos = (pos+4)%size;
                if (pos < stpos){fl =1;}
                id = decodeUnsignedInt(f);
                bd.seek(infolen + (id-1)*strclen + strcpos);
                QString check = bd.read(4);
                if (check==value) {
                    res.push_back(id);
                    if (ifkey){
                        res.push_back(0);
                        return res;
                    }
                }
            }

        }

        if (type == "float") {
            float val = decodeFloat(value);
            int valpos = decodeInt(encodeFloat(val));
            pos = (4*valpos)%size;
            stpos = pos;
            int fl =0;
            while (true) {
                file.seek(pos);
                QByteArray bytes(4,'\0');
                f = file.read(4);
                if(f==bytes or (pos == stpos and fl ==1)) {
                    res.push_back(0);
                    return res;
                }
                pos = (pos+4)%size;
                if (pos < stpos){fl =1;}
                id = decodeUnsignedInt(f);
                bd.seek(infolen + (id-1)*strclen + strcpos);
                QString check = bd.read(4);
                if (check==value) {
                    res.push_back(id);
                    if (ifkey){
                        res.push_back(0);
                        return res;
                    }
                }
            }
        }

        if (type == "bool") {
            int val = decodebool(value) == "true" ? 1 : 0;
            pos = (4*(val*size/2))%size;
            stpos = pos;
            int fl =0;
            while (true) {
                file.seek(pos);
                QByteArray bytes(4,'\0');
                f = file.read(4);
                if(f==bytes or (pos == stpos and fl ==1)) {
                    res.push_back(0);
                    return res;
                }
                pos = (pos+4)%size;
                if (pos < stpos){fl =1;}
                id = decodeUnsignedInt(f);
                bd.seek(infolen + (id-1)*strclen + strcpos);
                QString check = bd.read(1);
                if (check== value) {
                    res.push_back(id);
                    if (ifkey){
                        res.push_back(0);
                        return res;
                    }
                }
            }
        }

        if (type == "str") {
            pos = StringHash(value, size)%size;
            pos = (pos*4)%size;
            stpos = pos;
            int fl =0;
            while (true) {
                file.seek(pos);
                QByteArray bytes(4,'\0');
                f = file.read(4);

                if(f==bytes or (pos == stpos and fl ==1)) {
                    res.push_back(0);
                    return res;
                }
                pos = (pos+4)%size;
                if (pos < stpos){fl =1;}

                id = decodeUnsignedInt(f);
                bd.seek(infolen + (id-1)*strclen + strcpos);
                QString check = bd.read(16);

                if (StringHash(check, size)==StringHash(value, size)) {
                    string stdchec = check.toStdString();
                    stdchec.resize(stdchec.length());
                    string stdval = value.toStdString();
                    stdval.resize(stdchec.length());
                    if (stdchec == stdval) {
                        cout<<"e"<<endl;
                        res.push_back(id);
                        if (ifkey){
                            res.push_back(0);
                            return res;
                        }
                    }
                }
            }
        }

    }
    return {0};
}


inline void deletevalue(QString pathfile, QByteArray value, QString type, int size, QString bdpath, int strclen, int infolen, int strcpos, int ifkey) {
    QFile file(pathfile);
    QFile bd(bdpath);
    size = size*8;
    unsigned int id = 0;
    if (file.open(QFile::ReadWrite) and bd.open(QFile::ReadWrite)) {
        int pos=0;
        int stpos=pos;
        QByteArray f;
        if (type == "int") {
            int val = decodeInt(value);
            pos = (val*4)%size;
            stpos = pos;
            int fl =0;
            while (true) {
                file.seek(pos);
                QByteArray bytes(4,'\0');
                f = file.read(4);
                if(f==bytes or (pos == stpos and fl ==1)) {

                    return ;
                }
                pos = (pos+4)%size;
                if (pos < stpos){fl =1;}
                id = decodeUnsignedInt(f);
                bd.seek(infolen + (id-1)*strclen + strcpos);
                QString check = bd.read(4);
                if (check==value) {
                    file.seek(pos);
                    QByteArray bytes(4,'\0');
                    file.write(bytes);
                    if (ifkey){
                        return ;
                    }
                }
                pos = (pos+4)%size;
            }

        }

        if (type == "float") {
            float val = decodeFloat(value);
            int valpos = decodeInt(encodeFloat(val));
            pos = (4*(valpos%size))%size;
            stpos = pos;
            int fl =0;
            while (true) {
                file.seek(pos);
                QByteArray bytes(4,'\0');
                f = file.read(4);
                if(f==bytes or (pos == stpos and fl ==1)) {

                    return ;
                }
                pos = (pos+4)%size;
                if (pos < stpos){fl =1;}
                id = decodeUnsignedInt(f);
                bd.seek(infolen + (id-1)*strclen + strcpos);
                QString check = bd.read(4);
                if (check==value) {
                    file.seek(pos);
                    QByteArray bytes(4,'\0');
                    file.write(bytes);
                    if (ifkey){
                        return ;
                    }
                }
                pos = (pos+4)%size;
            }
        }

        if (type == "bool") {
            int val = decodebool(value) == "true" ? 1 : 0;
            if (val==1) {
                pos = size/2;
            }
            stpos = pos;
            int fl =0;
            while (true) {
                file.seek(pos);
                QByteArray bytes(4,'\0');
                f = file.read(4);
                if(f==bytes or (pos == stpos and fl ==1)) {

                    return ;
                }
                pos = (pos+4)%size;
                if (pos < stpos){fl =1;}
                id = decodeUnsignedInt(f);
                bd.seek(infolen + (id-1)*strclen + strcpos);
                QString check = bd.read(1);
                if (check== value) {
                    file.seek(pos);
                    QByteArray bytes(4,'\0');
                    file.write(bytes);
                    if (ifkey){
                        return ;
                    }
                }
                pos = (pos+4)%size;
            }
        }

        if (type == "str") {
            pos = StringHash(value, size);
            pos = (pos*4)%size;
            stpos = pos;
            int fl =0;
            while (true) {
                file.seek(pos);
                QByteArray bytes(4,'\0');
                f = file.read(4);
                if(f==bytes or (pos == stpos and fl ==1)) {

                    return ;
                }
                pos = (pos+4)%size;
                if (pos < stpos){fl =1;}
                id = decodeUnsignedInt(f);
                bd.seek(infolen + (id-1)*strclen + strcpos);
                QString check = bd.read(16);
                if (StringHash(check, size)==StringHash(value, size)) {
                    string stdchec = check.toStdString();
                    stdchec.resize(stdchec.length());
                    string stdval = value.toStdString();
                    stdval.resize(stdchec.length());
                    if (stdchec == stdval) {
                        file.seek(pos);
                        QByteArray bytes(4,'\0');
                        file.write(bytes);
                        if (ifkey){
                            return ;
                        }
                    }
                }
                pos = (pos+4)%size;
            }
        }

    }
    return;
}



inline QString decodeOutput(const QStringList& structure, int strc_len, QByteArrayList coded) {
    QString result = "";
    result += structure.join(" ") +'\n';
    QMap<QString, int> lens = {{"int", 4}, {"str", 16}, {"bool", 1}, {"float", 4}};
    for (auto str : coded) {
        int id = decodeUnsignedInt(str.sliced(0, 4));
        if (id == 0) {
            continue;
        }

        result += QString("%1").arg(id);
        result+='\t';
        int psl = 4;
        for (int i = 4; i < structure.size(); i+=3) {
            if (structure[i]=="int") {
                result += QString("%1").arg(decodeInt(str.sliced(psl, 4)));
                result += "\t";
                psl+=4;
            }
            if (structure[i]=="bool") {
                result += decodebool(str.sliced(psl, 1));
                result += "\t";
                psl+=1;
            }
            if (structure[i]=="str") {
                result += str.sliced(psl, 16).replace('\0',' ');
                result += "\t";
                psl+=16;
            }
            if (structure[i]=="float") {
                result += QString("%1").arg(decodeFloat(str.sliced(psl, 4)));
                result += "\t";
                psl+=4;
            }

        }
        result+='\n';
    }

    return result;
}


inline QByteArray univencoder(QString value, QString type) {
    if (type == "int") {
        int val = value.toInt();
        return encodeInt(val);
    }
    if (type == "float") {
        float val = value.toFloat();
        return encodeFloat(val);
    }
    if (type == "bool") {
        bool val = value.toInt();
        return encodeBool(val);
    }
    if (type == "str") {
        return value.slice(16).toUtf8();
    }
}


inline void writevalue(QString path, QString value, QString type, int size, int id) {
    QFile file(path);
    size =size*8;
    if (file.open(QFile::ReadWrite)) {
        int pos=0;

        if (type == "int") {
            int val = value.toInt();
            pos = (val*4)%size;
            cout<<pos<<endl;
            file.seek(pos);
            QByteArray bytes(4,'\0');
            QByteArray bytes2(4,255);
            while (true) {
                QByteArray f =file.read(4);
                if (f==bytes or f==bytes2) {
                    break;
                }
                pos =(pos+4)%size;
                file.seek(pos);
            }

        }

        if (type == "float") {
            float val = value.toFloat();
            int valpos = decodeInt(encodeFloat(val));
             pos = (4*valpos)%size;
            QByteArray bytes(4,'\0');
            QByteArray bytes2(4,255);
            while (true) {
                QByteArray f =file.read(4);
                if (f==bytes or f==bytes2) {
                    break;
                }
                pos =(pos+4)%size;
                file.seek(pos);
            }
        }

        if (type == "bool") {
            int val = value.toInt();
            if (val==1) {
                pos = size/2;
            }
            file.seek(pos);
            QByteArray bytes(4,'\0');
            QByteArray bytes2(4,255);
            while (true) {
                QByteArray f =file.read(4);
                if (f==bytes or f==bytes2) {
                    break;
                }
                pos =(pos+4)%size;
                file.seek(pos);
            }
        }
        if (type == "str") {
            pos = StringHash(value, size)%size;
            pos = (pos*4)%size;
            file.seek(pos);
            QByteArray bytes(4,'\0');
            QByteArray bytes2(4,255);
            while (true) {
                QByteArray f =file.read(4);
                if (f==bytes or f==bytes2) {
                    break;
                }
                pos =(pos+4)%size;
                file.seek(pos);
            }
        }
        file.seek(pos);
        cout<<pos<<endl;
        QByteArray ba = encodeUnsignedInt(id);
        file.write(ba);
    }
}






#endif //DBSYS_CODERS_H