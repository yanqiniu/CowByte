#ifndef _DRAW_CALL_H
#define _DRAW_CALL_H

enum DrawCallType
{

};

class DrawCall
{
public:
    enum Type
    {
        DrawCall_GameObject,
        DrawCall_Invalid
    };

    Type GetType() { return m_Type; }

protected:
    Type m_Type;
    //bool m_bInitialized = false;
    
};

#endif // 
