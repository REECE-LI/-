#ifndef _VCAN_MENU_H_
#define _VCAN_MENU_H_


typedef  enum
{
    RESULT_NULL,
    RESULT_INIT,            //重新初始化
    RESULT_TO_MENU,         //上一层 菜单
    SAVE,
    READ,
}result_e;

typedef  result_e  ap_func(void);

typedef struct
{
    void    *   strbuff;      //字符串图案
    uint32      strnum;       //中文字符数目
    ap_func *   pfunc;        //执行函数

}menu_t;

#define VCAN_MENU(s,fc)        {(void *)(s),sizeof(s)/LCD_CH_SIZE,fc}         //菜单，第一个参数是 汉字字库 ，第二个是 执行函数


#define LCD_FSTR_CH_FUNC     LCD_FStr_CH

extern void        menu_init(Site_t site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab);
extern void        pid_init(Site_t site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab);
extern result_e    menu_deal(Site_t site,menu_t *menu,uint8 tab,uint8 *new_tab,uint8 max_tab);
extern result_e    pid_deal_s(Site_t site,menu_t *menu,uint8 tab,uint8 *new_tab,uint8 max_tab);
extern void        menu_display(Site_t  site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab);



#endif  //_VCAN_MENU_H_