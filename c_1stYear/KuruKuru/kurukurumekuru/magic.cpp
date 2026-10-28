#include "magic.h"
#include "Enemy.h"
#include "Battle.h"

int fonts[FONTS_NUM];  // 配列の宣言
int countOfFoundChar;

int DoTDamage = 0;    //  継続ダメージ(捲るたびにダメージを与える)
int rateOfAddEfect = 0; //  追加効果発生率(水魔法用)  

int selectedMagicNo = 0;	//	選択された呪文No


void InitializeFonts()
{
    fonts[FONT_INDEX_MAGIC_NAME] = CreateFontToHandle(NULL, 25, -1);
    fonts[FONT_INDEX_MAGIC_SPEL] = CreateFontToHandle(NULL, 10, -1);
    fonts[FONT_INDEX_DMG_MP] = CreateFontToHandle(NULL, 20, -1);
    fonts[FONT_INDEX_PAGE_NO] = CreateFontToHandle(NULL, 16, -1);
}

//  完成版
Magic magic[9] =
{
    {0, "第一の火炎魔法", "Ignite the flame, burn with fury", 10, NEED_MP_FIRST,1},
    {1, "第二の火炎魔法", "Flame of wrath, set the skies ablaze!\nLet fire consume all in its path!", 10, NEED_MP_SECOND,1},
    {2, "第三の火炎魔法", "Inferno of destruction, heed my call!\nBurn with the wrath of a thousand suns,\nAnnihilate all before me!", 10, NEED_MP_THIRD,1},
    {3, "第一の風魔法", "Winds, rise and guide my path!", 10, NEED_MP_FIRST,1},
    {4, "第二の風魔法", "Storm of skies, rend through heavens!\nHowl, winds, and rage with fury!", 10, NEED_MP_SECOND - 10,1},
    {5, "第三の風魔法", "Tempest of the ages, answer my plea!\nUnleash the fury of the wind gods,\nLet the world be torn asunder!", 10, NEED_MP_THIRD - 10,1},
    {6, "第一の水魔法", "Waters, rise and cleanse the land!", 10, NEED_MP_FIRST,1},
    {7, "第二の水魔法", "Tide of the deep, flood the world!\nLet the oceans overflow and drown!", 10, NEED_MP_SECOND,1},
    {8, "第三の水魔法", "Great abyssal flood, I summon thee!\nWash the fabric of reality away,\nDrown all in the endless abyss!", 10, NEED_MP_THIRD,1}
};

//  現在覚えている詠唱文
Magic memorizedMagic[9] =
{
    {0, "第一の火炎魔法", "Ignite the flame, burn with fury", 10, NEED_MP_FIRST,0},
    {1, "第二の火炎魔法", "Flame of wrath, set the skies ablaze!\nLet fire consume all in its path!", 10, NEED_MP_SECOND,0},
    {2, "第三の火炎魔法", "Inferno of destruction, heed my call!\nBurn with the wrath of a thousand suns,\nAnnihilate all before me!", 10, NEED_MP_THIRD,0},
    {3, "第一の風魔法", "Winds, rise and guide my path!", 10, NEED_MP_FIRST,0},
    {4, "第二の風魔法", "Storm of skies, rend through heavens!\nHowl, winds, and rage with fury!", 10, NEED_MP_SECOND - 10,0},
    {5, "第三の風魔法", "Tempest of the ages, answer my plea!\nUnleash the fury of the wind gods,\nLet the world be torn asunder!", 10, NEED_MP_THIRD - 10,0},
    {6, "第一の水魔法", "Waters, rise and cleanse the land!", 10, NEED_MP_FIRST,0},
    {7, "第二の水魔法", "Tide of the deep, flood the world!\nLet the oceans overflow and drown!", 10, NEED_MP_SECOND,0},
    {8, "第三の水魔法", "Great abyssal flood, I summon thee!\nWash the fabric of reality away,\nDrown all in the endless abyss!", 10, NEED_MP_THIRD,0}
};

int pageList[256] =
{
    5, 0, 8, 4, 1, 7, 8, 6, 0, 0, 5, 1, 5, 2, 2, 7, 2, 7, 2, 7, 6, 8, 7, 5,
    1, 3, 6, 3, 4, 8, 3, 3, 6, 8, 4, 6, 0, 4, 7, 2, 5, 1, 3, 0, 6, 8, 2, 6,
    4, 8, 1, 4, 2, 1, 3, 1, 2, 7, 5, 7, 6, 8, 4, 4, 5, 4, 1, 6, 1, 0, 4, 6,
    2, 7, 0, 8, 3, 2, 6, 3, 8, 8, 6, 0, 8, 8, 6, 7, 4, 4, 1, 7, 8, 2, 8, 1,
    0, 5, 1, 2, 3, 1, 0, 4, 5, 0, 2, 5, 0, 6, 3, 1, 3, 0, 2, 5, 4, 2, 7, 3,
    1, 5, 1, 6, 3, 0, 7, 0, 4, 5, 3, 6, 6, 5, 4, 0, 4, 8, 7, 2, 7, 0, 4, 6,
    0, 3, 6, 0, 5, 3, 6, 7, 6, 0, 7, 5, 2, 4, 1, 8, 7, 8, 5, 3, 4, 2, 1, 0,
    1, 2, 1, 3, 3, 7, 4, 5, 8, 5, 2, 6, 4, 5, 2, 6, 0, 6, 4, 7, 5, 3, 8, 6,
    6, 8, 4, 6, 6, 7, 8, 3, 0, 0, 2, 0, 0, 3, 3, 3, 3, 7, 5, 8, 8, 8, 5, 4,
    5, 3, 0, 5, 2, 7, 7, 7, 8, 2, 5, 0, 0, 7, 4, 4, 0, 1, 1, 3, 3, 2, 1, 2,
    2, 5, 6, 7, 8, 2, 3, 1, 8, 4, 1, 7, 5, 1, 1, 1
};

void ResetMemory() //  覚えている詠唱文初期化
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; magic[i].spell[j] != '\0'; j++)
        {
            if (magic[i].spell[j] != ' ' && magic[i].spell[j] != '\n')
            {
                memorizedMagic[i].spell[j] = '0';
            }
        }
        memorizedMagic[i].isComplete = false;   //  コンプリート状態初期化
    }


}

int ContrastSpell(int index)    //  詠唱文の比較
{
    countOfFoundChar = 0;
    //	1文字ずつ比較して何文字開放したかカウント
    for (int i = 0; memorizedMagic[pageList[index]].spell[i] != '\0'; i++)
    {
        if (memorizedMagic[pageList[index]].spell[i] == magic[pageList[index]].spell[i])
        {
            //  空白以外カウント
            if (memorizedMagic[pageList[index]].spell[i] != ' ' && memorizedMagic[pageList[index]].spell[i] != '\n')
                countOfFoundChar++;
        }
    }
    return countOfFoundChar;
}

void CheckComplete(int index)   //  コンプリート状態を確認
{
    for (int i = 0; memorizedMagic[pageList[index]].spell[i] != '0'; i++)
    {
        if (memorizedMagic[pageList[index]].spell[i] == '\0')
        {
            //  末尾まで0がなければコンプリート
            memorizedMagic[pageList[index]].isComplete = 1;
            break;
        }
    }
}

int GetMagicDamage(int magicNo)
{
    if (magicNo < 0 || magicNo > 8)
        return 0;
    else
        return memorizedMagic[magicNo].damage;
}


void AdditionalEffect(int magicNo)
{
    rateOfAddEfect = 0; //  追加効果発生率初期化

    switch (magicNo)
    {   
    case 1: //  第二の火炎
    {
        DoTDamage = MAX_DOT_SECOND; //  継続ダメージ
        addEffectGraphIndex = INDEX_OF_FIRE_EFFECT_GRAPH;  //	追加効果描画用
    }
    break;
    case 2:  //  第三の火炎
    {
        DoTDamage = MAX_DOT_THIRD; //  継続ダメージ
        addEffectGraphIndex = INDEX_OF_FIRE_EFFECT_GRAPH; //	追加効果描画用
    }
    break;
    case 7:  //  第二の水
        rateOfAddEfect = 20;    //  追加効果発生率
        break;
    case 8: //  第三の水
        rateOfAddEfect = 40;    //  追加効果発生率 
        break;
    }

    if (rateOfAddEfect == 0)
        return;
    else
    {
        if (GetRand(99) < rateOfAddEfect)    //  0-発生率未満で成功
        {
            turnsUntilCurrentAction++;
            addEffectGraphIndex = INDEX_OF_WATER_EFFECT_GRAPH; //	追加効果描画用
        }
    }
}

void DoT()  //  継続ダメージ
{
    if (DoTDamage > 0)  //  DoTDamageがあるならダメージを与える
    {
        enemyStatusList.currentHP[encounteredEnemy] -= DoTDamage;	//	ダメージ処理
        if (enemyStatusList.currentHP[encounteredEnemy] < 0)    //  HP下限
            enemyStatusList.currentHP[encounteredEnemy] = 0;
    
        DoTDamage -= 5; //  継続ダメージは5ずつ減少
    
        KillEnemy();
    }
}

void DrawMagicIcon(int posX, int posY,int index)    //  アイコン描画
{
    int magicType = memorizedMagic[pageList[index]].no / 3;   //  魔法属性
    int magicLevel = memorizedMagic[pageList[index]].no % 3;  //  魔法階級(第1～3)
    
    
    //  属性ごとに輝度設定
    switch (magicType)
    {
        case 0: //  火
            SetDrawBright(255,99,71);
            break;
        case 1: //  風
            SetDrawBright(144, 238, 144);
            break;
        case 2: //  水
            SetDrawBright(135, 206, 250);
            break;
    }

    switch (magicLevel) //  階級ごとに描画追加
    {
    case 2: //  第三
        DrawCircleAA(posX, posY, 6, 16, 0xffffff, false);
    case 1: //  第二
        DrawCircleAA(posX, posY, 9, 16, 0xffffff, false);
    case 0: //  第一
        DrawCircleAA(posX, posY, 12, 16, 0xffffff, false);
        break;
    }

    float angle = 0;    //  角度
    static const double pi = 3.141592653589793;	//	π

    for (int i = 0; i < 6; i++)//	円内の線
    {
        DrawLineAA(posX, posY, posX + 12 * cos(pi * (angle / 180.0)), posY + 12 * sin(pi * (angle / 180.0)), 0xffffff, 1.0F);
        DrawLineAA(posX, posY, posX - 12 * cos(pi * (angle / 180.0)), posY - 12 * sin(pi * (angle / 180.0)), 0xffffff, 1.0F);
        angle += 30;
    }
    SetDrawBright(255, 255, 255);   //  輝度を戻す
}
