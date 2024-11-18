#ifndef MENU_DISP_H__
#define MENU_DISP_H__

#define SUPER_PASS 2222
#define PARA_MENU_NUM 14
#define SET_MENU_NUM 8
#define SUPER_SET_MENU_NUM 10
enum{
    PARA,
    PASSWORD,
    SET,
    SUPER_SET,
    PRINT
};
enum{
    SET_PASS,
    SET_ADDR,
    SET_BAUD,
    SET_CT,
    SET_PT,
    SET_SYSMODE,
    SET_OVER_V,
    SET_LOW_V,
    //SET_V_PREC,
    //SET_A_PREC,
    SET_SOFT  
};
enum{
    SUPER_SET_PASS,
    SUPER_SET_ADDR,
    SUPER_SET_BAUD,
    SUPER_SET_CT,
    SUPER_SET_PT,
    SUPER_SET_OVER_V,
    SUPER_SET_LOW_V,
    SUPER_SET_V_PREC,
    SUPER_SET_A_PREC,
    SUPER_SET_CLR,
    SUPER_SET_SOFT,  
    SUPER_SET_SYSMODE
};
enum{
    KEY_MODE = 1,
    KEY_UP,
    KEY_DOWN,
    KEY_EXIT,
    KEY_PRINT
};
void IntoParaMenu(void);
void MenuProcess(void);

#endif

