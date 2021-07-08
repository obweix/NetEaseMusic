#ifndef SKINCONFIG_H
#define SKINCONFIG_H

#include<QtWidgets>

QT_BEGIN_NAMESPACE
//class QString;
QT_END_NAMESPACE

class SkinConfig
{
public:
    static SkinConfig& getInstance()
    {
        static SkinConfig skinConfigInstance;
        return skinConfigInstance;
    }

    QString getConfig();


private:
    SkinConfig();
    SkinConfig(const SkinConfig& cfg)=delete ;
    SkinConfig& operator=(const SkinConfig& cfg)=delete ;

};

#endif // SKINCONFIG_H
