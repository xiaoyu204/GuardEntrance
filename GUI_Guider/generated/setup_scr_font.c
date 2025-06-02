/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_font(lv_ui *ui)
{
    //Write codes font
    ui->font = lv_obj_create(NULL);
    lv_obj_set_size(ui->font, 800, 480);
    lv_obj_set_scrollbar_mode(ui->font, LV_SCROLLBAR_MODE_OFF);

    //Write style for font, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes font_label_font
    ui->font_label_font = lv_label_create(ui->font);
    lv_label_set_text(ui->font_label_font, "系代加各很教新向机她内此老变原结问手日利质已最政儿见并平资比特果什建反常知第电思立提?或通解身四品几位别论公给少条观回海基次被山才己期西术济认先命走真员及数话门级军统光声题入美口感战科程式指世必放打接总做东区农强造类受场五直月流决干则更色处路运任具目再治神求件管组根阶将改导眼规识革计白马金界取市设语完究党女传风信名便保育队带叫研领北较张即至许步往听调务花争线呢每边难太共交确劳据达住收候需转百南清格影书切且却志热联安极今单商料技深验增记近言整精集空连报觉车价音响办存怎病快图况例消容史非离节亲万八构族石满何广律青林克王历权素始断九际积吃态艺证众创红望须群师该复细包土持服笑德般远爱准写算火死半布随六元低称引照失养视习段字织斗团器兴乐效显斯千落示仅企似备除支标早吧周速跟七采状吗约城层专划轻拉值适英告讲维营士环古让院按型势足毛台紧片属严树画厂功注演源温某找参易飞推围列客河虽境食李尔黄黑念越选华角考致夫初装议首委底另江息密压孩村费局派房星突供曾排苦够像。，站校富谈米充阳破母球射批配纪未哪妈差刚率察舞依占呀微础益倒刻举血故互范木号尽请脸兵职留铁吸钱急独剧查皮激坐乎固害夜续京双拿府限草项述曲既春官坚令句助简杂材封护司省县模试绝洋谁继止喜优词陈鱼终施晚朝含仍脑甚汉修油衣击云送巴普征错均投波余友武责游敌叶船久否异块脚怕田策苏乡帝靠医测植啊括换逐娘罗戏善获待降冲父置左右沉酸亮良班著亚抗啦静渐概居跑旧防买挥负帮欢冷液停减男胜答析忙短烈映财背奇药承岁略穿牛室移饭坏假州缺愿练超雨慢竟呼散灵副歌谓虫读介培卫宗章评伤丰核预担诉惊刘景乱氧阿龙宣升爷洲判案协福货味若促损秀丽秋执束纳虑疑午粮架盛湖尚床份套毫奋扬赵混献择庄殖店皇购缩盐冬齐径偏盖款衡阻托喊繁纯牙殊浪招符折泽弄奶谢累亦厚朋伸筑闪枝授蛋潮登售孔街败迎肥恩尼幼惯钢肯矿岛伟粉振吴夺尤忘荣亡阴凡毕暴唐勇避麻股硬洞盘轮警闹录绍描寻妹幸壁距玩休延综甲泛哲载雷窗虚秘俗倾哭禁零触巧圈努喝彻晶丁操奴宜菌叔桥仿默航谷泪予挂壮纷粒燃诸削苗婆付迷馆灰鸡岸寒泥梅残税虎庆抱鸟拍墙缓横尾督莫械尖旅倍措剥遭懂粗奏润侧辩茶鬼梦贯篇妻宽腿刀桌浓跃蹈洗卷欲袋赏悲劲鲁赞氏汇迹竞醒裂闭森腰磁湿聚兄徐炼订川吹蒙胸召伴徒幅疗淡奖仪珠绪蒋猪剂访狗摸趋宋伏棉恐羊碳隔网猛爬缘麦库津监薄净旋贫罢穷乏挑废汗耗尊牌抢软隶宇圣港欣塞刑博揭垂姓捕荒诚炮呈辑卡伦蓝抬宫敬版忍颜竹碰启票隐抵脉浮煤剩珍奔堆扎谋貌脏逃洪旗潜鼻忆湾绩恰坦怒震豆插戴涉绘瓦龄池沟呵伍氢拥乘吉尺朱蒸舍贡糊梁漫腾症丈钻磨井艰键绕幕斤污贝冒阔胆渡碍崇愈岩玻碎宪俄障融贸耐届寄荡币俩卵弃悄偶纵陷坡勤享塑膜铜拖暖稍摩硫哩乌循妙疾肩炎偷赤灾恒孤仁腔杯乃稀恢违伊挺乙拜焦锋铺颗滑敏洁嘛凝凉杜凭勒爆递鞋迁秦桂璃摄叹峰牧恨糖骂贴紫壤辛仰腐割辈君泡郭呆番励乳亿兼昨希握端针屋掌检痛闻妇贵顾沙扩困哥嘴销印免烟跳香纸户板睛误探座盾灯搞祖顺婚赶季兰忽践熟矛银爸临佛犯胞讨陆补童圆归弟域索罪脱追守课顶松杀掉野训遍庭弱赛额酒绿席露卖钟旁肉央染欧怀稳伯编抓玉咱唯唱翻释控怪阵烧堂杨您诗灭夏敢退姑恶透鲜附骨审胡输宁丝姐彩睡巨迫鼓伙借耳典刺暗镜序哈菜溶笔汽礼摇迅险遗威沿弹私攻宝抽雪的一是不了在有人这上大来和我个中地为他生要们以到国时就出说会也子学发着对作能可于成用过动主下而年分得家种里多经自现同后产方工行面那小所起去之都然理进体还定实如么物法你好性民从天化等力本长心把部义样事看业当因高十开些社前又它水其没想意三只重点与使但度由道全制明相两情外间二关活正合者形应头无量表象气文展孙藏毒顿末摆趣救镇楼智园康永遇雄替危楚侵枪刊擦迟拔丹冰健播途彼搬滴朵畜遵逻患");
    lv_label_set_long_mode(ui->font_label_font, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->font_label_font, 0, 0);
    lv_obj_set_size(ui->font_label_font, 800, 480);
    lv_obj_add_flag(ui->font_label_font, LV_OBJ_FLAG_HIDDEN);

    //Write style for font_label_font, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->font_label_font, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->font_label_font, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->font_label_font, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->font_label_font, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->font_label_font, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of font.


    //Update current screen layout.
    lv_obj_update_layout(ui->font);

}
