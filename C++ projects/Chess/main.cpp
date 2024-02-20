#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace sf;

Font font;

class Switch
{
public:
    Switch() {}
    Switch(int x, int y, Image& img, Texture& tex, Sprite& spr, std::string s)
    {
        rect.setPosition(x,y);
        rect.setSize(Vector2f (30,30));
        rect.setFillColor(Color(255,255,255));
        rect.setOutlineThickness(2);
        rect.setOutlineColor(Color(0,0,0));
        img_check = img;
        tex_check = tex;
        spr_check = spr;
        spr_check.setPosition(x,y);
        text.setFont(font);
        text.setString(s);
        text.setCharacterSize(18);
        text.setColor(Color(0,0,0));
        text.setPosition(x+40,y+2);
        check = 1;
    }
    Switch(int x, int y, std::string s)
    {
        rect.setPosition(x,y);
        rect.setSize(Vector2f (30,30));
        rect.setFillColor(Color(255,255,255));
        rect.setOutlineThickness(2);
        rect.setOutlineColor(Color(0,0,0));
        text.setFont(font);
        text.setString(s);
        text.setCharacterSize(18);
        text.setColor(Color(0,0,0));
        text.setPosition(x+40,y+2);
        check = 0;
        click = 0;
    }
    bool update(RenderWindow& app)
    {
        Vector2i pos = Mouse::getPosition(app);
        app.draw(rect);
        app.draw(text);
        if (!Mouse::isButtonPressed(Mouse::Left))
            done = 1;
        if (Mouse::isButtonPressed(Mouse::Left) && pos.x>rect.getPosition().x && pos.y>rect.getPosition().y && pos.x<rect.getSize().x+rect.getPosition().x && pos.y<rect.getSize().y+rect.getPosition().y)
        {
            if (done)
            {
                if (click)
                    click = 0;
                else
                    click = 1;
                done = 0;
            }
        }
        if (click)
        {
            if (check)
                app.draw(spr_check);
            else
                rect.setFillColor(Color(0,0,0));
            return 1;
        }
        else
        {
            if (!check)
                rect.setFillColor(Color(255,255,255));
            return 0;
        }
    }
    void draw(RenderWindow& app)
    {
        app.draw(rect);
        app.draw(text);
        if (click && check)
            app.draw(spr_check);
    }
    void change_name(std::string s) {text.setString(s);}
private:
    RectangleShape rect;
    Image img_check;
    Texture tex_check;
    Sprite spr_check;
    Text text;
    bool click = 1, done = 1, check;
};

class Button
{
public:
    Button() {}
    Button(int x, int y, int sizex, int sizey, std::string txt)
    {
        rect.setPosition(x,y);
        rect.setSize(Vector2f (sizex,sizey));
        rect.setFillColor(Color(224,224,224));
        rect.setOutlineThickness(2);
        rect.setOutlineColor(Color(0,0,0));
        text.setFont(font);
        text.setString(txt);
        text.setCharacterSize(18);
        text.setColor(Color(0,0,0));
        text.setPosition((sizex-6)/2-(txt.size()*5)+x+3,(sizey-6)/2+y-9);
    }
    bool update(RenderWindow& app, bool on_down)
    {
        Vector2i pos = Mouse::getPosition(app);
        if (on_down)
        {
            if (click)
                rect.setFillColor(Color(224,224,224));
            else
                rect.setFillColor(Color(128,128,128));
            app.draw(rect);
            app.draw(text);
            if (click && Mouse::isButtonPressed(Mouse::Left) && pos.x>rect.getPosition().x && pos.y>rect.getPosition().y && pos.x<rect.getSize().x+rect.getPosition().x && pos.y<rect.getSize().y+rect.getPosition().y)
            {
                click = 0;
                return 0;
            }
            else
            {
                if (!click && !Mouse::isButtonPressed(Mouse::Left) && pos.x>rect.getPosition().x && pos.y>rect.getPosition().y && pos.x<rect.getSize().x+rect.getPosition().x && pos.y<rect.getSize().y+rect.getPosition().y)
                {
                    click = 1;
                    return 1;
                }
                else
                {
                    if (!Mouse::isButtonPressed(Mouse::Left) || !(pos.x>rect.getPosition().x && pos.y>rect.getPosition().y && pos.x<rect.getSize().x+rect.getPosition().x && pos.y<rect.getSize().y+rect.getPosition().y))
                        click = 1;
                    return 0;
                }
            }
        }
        else
        {
            if (!Mouse::isButtonPressed(Mouse::Left))
                click = 1;
            if (click)
                rect.setFillColor(Color(224,224,224));
            else
                rect.setFillColor(Color(128,128,128));
            app.draw(rect);
            app.draw(text);
            if (click && Mouse::isButtonPressed(Mouse::Left) && pos.x>rect.getPosition().x && pos.y>rect.getPosition().y && pos.x<rect.getSize().x+rect.getPosition().x && pos.y<rect.getSize().y+rect.getPosition().y)
            {
                click = 0;
                return 1;
            }
            else
                return 0;
        }
    }
    void draw(RenderWindow& app)
    {
        app.draw(rect);
        app.draw(text);
    }
private:
    RectangleShape rect;
    Text text;
    bool click = 1;
};

class Fig
{
public:
    Fig() {state = '0';}
    Fig(Image& i, Texture& t, Sprite& sp, char S,int X, int Y)
    {
        img = i;
        tex = t;
        spr = sp;
        x = 100*X+4;
        y = 100*Y+4;
        state = S;
    }
    void create(std::string F,char S,int X, int Y)
    {
        img.loadFromFile("img/" + F);
        img.createMaskFromColor(Color(255,255,255));
        tex.loadFromImage(img);
        spr.setTexture(tex);
        x = 100*X+4;
        y = 100*Y+4;
        state = S;
    }
    void draw(RenderWindow& app)
    {
        if (state != '0')
        {
            spr.setPosition(x,y);
            app.draw(spr);

        }
    }
    void follow_mouse(Vector2i pos)
    {
        x = pos.x-49;
        y = pos.y-49;
    }
    void release(int ax, int ay)
    {
        x = 100*((ax-3)/100)+4;
        y = 100*((ay-3)/100)+4;
    }
    void eaten()
    {
        x = -100;
        y = -100;
        state = '0';
    }
    void rejoin(int cx, int cy, char cstate)
    {
        x = cx;
        y = cy;
        state = cstate;
    }
    void promotion(char c, int player_move)
    {
        state = c;
        String F;
        if (player_move == 0)
            F+='w';
        else
            F+='b';
        if (c == 'q')
            F+="queen.bmp";
        else
            if (c == 'r')
                F+="rook.bmp";
            else
                if (c == 'b')
                    F+="bishop.bmp";
                else
                    if (c == 'n')
                        F+="knight.bmp";
        img.loadFromFile("img/" + F);
        img.createMaskFromColor(Color(255,255,255));
        tex.loadFromImage(img);
        spr.setTexture(tex);
    }
    int get_x() {return x;}
    int get_y() {return y;}
    char get_state() {return state;}
private:
    Image img;
    Texture tex;
    Sprite spr;
    char state;
    int x,y;
};

struct Arrow
{
    int x1, y1, x2, y2;
    RectangleShape body;
    ConvexShape arrow;
};

struct Notes
{
    std::vector <Text> moves;
    RectangleShape rect;
    std::vector <Text> num;
};

class Game_board
{
public:
    Game_board(std::vector <std::vector <Fig>> x)
    {
        figures = x;
        load();

        rectangle = RectangleShape(Vector2f(800,800));
        rectangle.setFillColor(Color(255,255,255,128));
        rectangle.setPosition(3,3);
        Arrow ar;
        arr.push_back(ar);
        arr[0].body = RectangleShape(Vector2f(100,10));
        arr[0].body.setFillColor(Color(180,225,30,230));
        arr[0].body.setPosition(-100,-100);
        arr[0].arrow.setPointCount(4);
        arr[0].arrow.setPoint(0,Vector2f(0,0));
        arr[0].arrow.setPoint(1,Vector2f(-10,30));
        arr[0].arrow.setPoint(2,Vector2f(50,0));
        arr[0].arrow.setPoint(3,Vector2f(-10,-30));
        arr[0].arrow.setFillColor(Color(180,225,30,250));
        arr[0].arrow.setPosition(-100,-100);

        line.setFont(font);
        line.setString("Moves:");
        line.setPosition(875,3);
        line.setCharacterSize(24);
        line.setColor(Color(0,0,0));
        show_fen[0].setFont(font);
        show_fen[0].setString("FEN: ");
        show_fen[0].setPosition(67,850);
        show_fen[0].setCharacterSize(18);
        show_fen[0].setColor(Color(0,0,0));
        show_fen[1].setFont(font);
        show_fen[1].setPosition(110,850);
        show_fen[1].setCharacterSize(18);
        show_fen[1].setColor(Color(0,0,0));
        game_over.setFont(font);
        game_over.setPosition(3,872);
        game_over.setCharacterSize(30);
        game_over.setColor(Color(0,0,0));

        copy_fen = Button (3,850,60,22,"copy");
        move_back = Button (873,850,100,22,"move back");
        restart = Button (1003,850,100,22,"restart");
        ex = Button (1127,3,60,27,"exit");

        n1.rect = RectangleShape (Vector2f (127,27));
        n1.rect.setPosition(-130,-30);
        n1.rect.setFillColor(Color (224,224,224));

        noting.push_back(RectangleShape (Vector2f (317,27)));
        noting[0].setPosition(870,3);
        noting[0].setFillColor(Color (255,255,255));
        noting[0].setOutlineColor(Color (0,0,0));
        noting[0].setOutlineThickness(2);
        noting.push_back(RectangleShape (Vector2f (317,803)));
        noting[1].setPosition(870,3);
        noting[1].setFillColor(Color (255,255,255));
        noting[1].setOutlineColor(Color (0,0,0));
        noting[1].setOutlineThickness(2);
        noting.push_back(RectangleShape (Vector2f (317,27)));
        noting[2].setPosition(870,808);
        noting[2].setFillColor(Color (255,255,255));
        noting.push_back(RectangleShape (Vector2f (317,2)));
        noting[3].setPosition(870,806);
        noting[3].setFillColor(Color(0,0,0));

        Xways = std::vector <std::vector <int>> (2);
        Yways = std::vector <std::vector <int>> (2);
        Xways[0] = {1,-1,1,-1,1,-1,0,0};
        Xways[1] = {-1,-1,1,1,0,0,1,-1};
        Yways[0] = {1,1,-1,-1,2,2,-2,-2};
        Yways[1] = {2,-2,2,-2,1,-1,1,-1};

        attack = std::vector <std::vector <int>> (8,std::vector <int> (8,0));
        marks = std::vector <std::vector <int>> (8,std::vector <int> (8,0));
        king = std::vector <bool> (figures.size(),0);
        rook = std::vector <bool> (2*figures.size(),0);
    }
    void load()
    {
        img_board.loadFromFile("img/board.bmp");
        tex_board.loadFromImage(img_board);
        spr_board.setTexture(tex_board);

        img_dot.loadFromFile("img/dot.bmp");
        img_dot.createMaskFromColor(Color(255,255,255));
        tex_dot.loadFromImage(img_dot);
        spr_dot.setTexture(tex_dot);

        img_mark.loadFromFile("img/mark.bmp");
        img_mark.createMaskFromColor(Color(255,255,255));
        tex_mark.loadFromImage(img_mark);
        spr_mark.setTexture(tex_mark);

        img_last.loadFromFile("img/last.bmp");
        img_last.createMaskFromColor(Color(255,255,255));
        tex_last.loadFromImage(img_last);
        spr_last.setTexture(tex_last);

        img_wpromote.loadFromFile("img/wpromote.bmp");
        tex_wpromote.loadFromImage(img_wpromote);
        spr_wpromote.setTexture(tex_wpromote);

        img_bpromote.loadFromFile("img/bpromote.bmp");
        tex_bpromote.loadFromImage(img_bpromote);
        spr_bpromote.setTexture(tex_bpromote);

        img_wqueen.loadFromFile("img/wqueen.bmp");
        img_wqueen.createMaskFromColor(Color(255,255,255));
        tex_wqueen.loadFromImage(img_wqueen);
        spr_wqueen.setTexture(tex_wqueen);
        img_wrook.loadFromFile("img/wrook.bmp");
        img_wrook.createMaskFromColor(Color(255,255,255));
        tex_wrook.loadFromImage(img_wrook);
        spr_wrook.setTexture(tex_wrook);
        img_wbishop.loadFromFile("img/wbishop.bmp");
        img_wbishop.createMaskFromColor(Color(255,255,255));
        tex_wbishop.loadFromImage(img_wbishop);
        spr_wbishop.setTexture(tex_wbishop);
        img_wknight.loadFromFile("img/wknight.bmp");
        img_wknight.createMaskFromColor(Color(255,255,255));
        tex_wknight.loadFromImage(img_wknight);
        spr_wknight.setTexture(tex_wknight);
        img_wpawn.loadFromFile("img/wpawn.bmp");
        img_wpawn.createMaskFromColor(Color(255,255,255));
        tex_wpawn.loadFromImage(img_wpawn);
        spr_wpawn.setTexture(tex_wpawn);
        img_wking.loadFromFile("img/wking.bmp");
        img_wking.createMaskFromColor(Color(255,255,255));
        tex_wking.loadFromImage(img_wking);
        spr_wking.setTexture(tex_wking);
        img_bqueen.loadFromFile("img/bqueen.bmp");
        img_bqueen.createMaskFromColor(Color(255,255,255));
        tex_bqueen.loadFromImage(img_bqueen);
        spr_bqueen.setTexture(tex_bqueen);
        img_brook.loadFromFile("img/brook.bmp");
        img_brook.createMaskFromColor(Color(255,255,255));
        tex_brook.loadFromImage(img_brook);
        spr_brook.setTexture(tex_brook);
        img_bbishop.loadFromFile("img/bbishop.bmp");
        img_bbishop.createMaskFromColor(Color(255,255,255));
        tex_bbishop.loadFromImage(img_bbishop);
        spr_bbishop.setTexture(tex_bbishop);
        img_bknight.loadFromFile("img/bknight.bmp");
        img_bknight.createMaskFromColor(Color(255,255,255));
        tex_bknight.loadFromImage(img_bknight);
        spr_bknight.setTexture(tex_bknight);
        img_bpawn.loadFromFile("img/bpawn.bmp");
        img_bpawn.createMaskFromColor(Color(255,255,255));
        tex_bpawn.loadFromImage(img_bpawn);
        spr_bpawn.setTexture(tex_bpawn);
        img_bking.loadFromFile("img/bking.bmp");
        img_bking.createMaskFromColor(Color(255,255,255));
        tex_bking.loadFromImage(img_bking);
        spr_bking.setTexture(tex_bking);
    }
    std::vector <int> find_fig(int x, int y)
    {
        std::vector <int> v(2,-1);
        for (int i=0;i<2;i++)
            for (int j=0;j<figures[i].size();j++)
                if ((figures[i][j].get_x()-4)/100 == x && (figures[i][j].get_y()-4)/100 == y)
                {
                    v[0] = i;
                    v[1] = j;
                    return v;
                }
        return v;  //empty
    }
    std::vector <int> find_fig(int x, int y, int player_move)
    {
        std::vector <int> v(2,-1);
        for (int j=0;j<figures[player_move].size();j++)
            if ((figures[player_move][j].get_x()-4)/100 == x && (figures[player_move][j].get_y()-4)/100 == y)
            {
                v[0] = player_move;
                v[1] = j;
                return v;
            }
        return v;  //empty
    }
    bool find_in_close(int kx, int ky)
    {
        if (close.size()==0)
            return 0;
        if (close[0].x == -1)
            return 0;
        else
            for (int i=0;i<close.size();i++)
                if (kx == close[i].x && ky == close[i].y)
                    return 1;
        return 0;
    }
    void block(int player_move, std::vector <int> v, int kx, int ky, int p, int i)
    {
        if (v[0] == 1-player_move)
            if (figures[v[0]][v[1]].get_state() == 'k')
            {
                if (close.size()>0)
                    close[0].x = -1;
                else
                    while (kx != (figures[player_move][i].get_x()-4)/100 || ky != (figures[player_move][i].get_y()-4)/100)
                    {
                        if (figures[player_move][i].get_state() == 'n')
                        {
                            kx-=Yways[0][p];
                            ky-=Yways[1][p];
                        }
                        else
                        {
                            kx-=Xways[0][p];
                            ky-=Xways[1][p];
                        }
                        Vector2i v;
                        v.x = kx;
                        v.y = ky;
                        close.push_back(v);
                    }
            }
    }
    void attacking(int player_move, bool doblock)
    {
        if (doblock)
            close.clear();
        for (int i=0;i<8;i++)
            for (int j=0;j<8;j++)
                attack[j][i] = 0;
        int kx, ky;
        for (int i=0;i<figures[player_move].size();i++)
        {
            if (figures[player_move][i].get_state()=='q')
            {
                for (int p=0;p<8;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    while (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                        if (v[0] != -1)
                        {
                            if (doblock)
                                block(player_move,v,kx,ky,p,i);
                            break;
                        }
                        kx+=Xways[0][p];
                        ky+=Xways[1][p];
                    }
                }

            }
            if (figures[player_move][i].get_state()=='b')
            {
                for (int p=0;p<4;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    while (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                        if (v[0] != -1)
                        {
                            if (doblock)
                                block(player_move,v,kx,ky,p,i);
                            break;
                        }
                        kx+=Xways[0][p];
                        ky+=Xways[1][p];
                    }
                }

            }
            if (figures[player_move][i].get_state()=='r')
            {
                for (int p=4;p<8;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    while (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                        if (v[0] != -1)
                        {
                            if (doblock)
                                block(player_move,v,kx,ky,p,i);
                            break;
                        }
                        kx+=Xways[0][p];
                        ky+=Xways[1][p];
                    }
                }

            }
            if (figures[player_move][i].get_state()=='n')
            {
                for (int p=0;p<8;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Yways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Yways[1][p];
                    if (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                        if (v[0] != -1)
                            if (doblock)
                                block(player_move,v,kx,ky,p,i);
                    }
                }

            }
            if (figures[player_move][i].get_state()=='p')
            {
                for (int p=0+2*player_move;p<2+2*player_move;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    if (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                        if (v[0] != -1)
                            if (doblock)
                                block(player_move,v,kx,ky,p,i);
                    }
                }

            }
            if (figures[player_move][i].get_state()=='k')
            {
                for (int p=0;p<8;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    if (kx>=0 && kx<8 && ky>=0 && ky<8)
                        attack[kx][ky] = 1;
                }

            }
        }
    }
    int find_king(int player_move)
    {
        for (int i=0;i<figures[player_move].size();i++)
            if (figures[player_move][i].get_state() == 'k')
                return i;
    }
    bool nocheck(int player_move,Fig& f)
    {
        int x = f.get_x(), y = f.get_y();
        char s = f.get_state();
        f.eaten();
        attacking(1-player_move,1);
        f.rejoin(x,y,s);
        if (attack[(figures[player_move][find_king(player_move)].get_x()-4)/100][(figures[player_move][find_king(player_move)].get_y()-4)/100] == 1)
            return 0;
        return 1;
    }
    void possible_moves(Fig& f,bool domarks,int player_move)
    {
        if (domarks)
        {
            for (int i=0;i<8;i++)
                for (int j=0;j<8;j++)
                    marks[j][i] = 0;
        }
        Vector2i w;
        moves.clear();
        w.x = (f.get_x()-4)/100;
        w.y = (f.get_y()-4)/100;
        moves.push_back(w);
        if (domarks)
            marks[w.x][w.y] = 3;
        int kx, ky;
        bool b = nocheck(player_move,f);
        std::vector <int> v;
        if (f.get_state() == 'q')
        {
            for (int p=0;p<8;p++)
            {
                kx = (f.get_x()-4)/100+Xways[0][p];
                ky = (f.get_y()-4)/100+Xways[1][p];
                while (kx>=0 && kx<8 && ky>=0 && ky<8)
                {
                    v = find_fig(kx,ky);
                    if (v[0] == player_move)
                        break;
                    else
                    {
                        if (b || find_in_close(kx,ky))
                        {
                            if (v[0] != -1)
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 2;
                                break;
                            }
                            else
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 1;
                                kx+=Xways[0][p];
                                ky+=Xways[1][p];
                            }
                        }
                        else
                        {
                            if (v[0] != -1)
                                break;
                            kx+=Xways[0][p];
                            ky+=Xways[1][p];
                        }
                    }
                }
            }
        }
        if (f.get_state() == 'b')
        {
            for (int p=0;p<4;p++)
            {
                kx = (f.get_x()-4)/100+Xways[0][p];
                ky = (f.get_y()-4)/100+Xways[1][p];
                while (kx>=0 && kx<8 && ky>=0 && ky<8)
                {
                    v = find_fig(kx,ky);
                    if (v[0] == player_move)
                        break;
                    else
                    {
                        if (b || find_in_close(kx,ky))
                        {
                            if (v[0] != -1)
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 2;
                                break;
                            }
                            else
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 1;
                                kx+=Xways[0][p];
                                ky+=Xways[1][p];
                            }
                        }
                        else
                        {
                            if (v[0] != -1)
                                break;
                            kx+=Xways[0][p];
                            ky+=Xways[1][p];
                        }
                    }
                }
            }
        }
        if (f.get_state() == 'r')
        {
            for (int p=4;p<8;p++)
            {
                kx = (f.get_x()-4)/100+Xways[0][p];
                ky = (f.get_y()-4)/100+Xways[1][p];
                while (kx>=0 && kx<8 && ky>=0 && ky<8)
                {
                    v = find_fig(kx,ky);
                    if (v[0] == player_move)
                        break;
                    else
                    {
                        if (b || find_in_close(kx,ky))
                        {
                            if (v[0] != -1)
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 2;
                                break;
                            }
                            else
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 1;
                                kx+=Xways[0][p];
                                ky+=Xways[1][p];
                            }
                        }
                        else
                        {
                            if (v[0] != -1)
                                break;
                            kx+=Xways[0][p];
                            ky+=Xways[1][p];
                        }
                    }
                }
            }
        }
        if (f.get_state() == 'n')
        {
            for (int p=0;p<8;p++)
            {
                kx = (f.get_x()-4)/100+Yways[0][p];
                ky = (f.get_y()-4)/100+Yways[1][p];
                if (kx>=0 && kx<8 && ky>=0 && ky<8)
                {
                    v = find_fig(kx,ky);
                    if (v[0] != player_move)
                    {
                        if (b || find_in_close(kx,ky))
                        {
                            if (v[0] != -1)
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 2;
                            }
                            else
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 1;
                            }
                        }
                    }
                }
            }
        }
        if (f.get_state() == 'p')
        {
            for (int p=0+2*player_move;p<2+2*player_move;p++)
            {
                kx = (f.get_x()-4)/100+Xways[0][p];
                ky = (f.get_y()-4)/100+Xways[1][p];
                if (kx>=0 && kx<8 && ky>=0 && ky<8)
                {
                    v = find_fig(kx,ky);
                    if (v[0] != player_move)
                    {
                        if (b || find_in_close(kx,ky))
                        {
                            if (v[0] != -1 || (enpassant.x == kx && enpassant.y == ky))
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 2;
                            }
                        }
                    }
                }
            }
            kx = (f.get_x()-4)/100;
            ky = (f.get_y()-4)/100+2*player_move-1;
            if (kx>=0 && kx<8 && ky>=0 && ky<8)
            {
                v = find_fig(kx,ky);
                if (v[0] != player_move)
                {
                    if (b || find_in_close(kx,ky))
                    {
                        if (v[0] == -1)
                        {
                            w.x = kx;
                            w.y = ky;
                            moves.push_back(w);
                            if (domarks)
                                marks[kx][ky] = 1;
                        }
                    }
                }
            }
            if (v[0] == -1)
            {
                kx = (f.get_x()-4)/100;
                ky = (f.get_y()-4)/100+4*player_move-2;
                if (kx>=0 && kx<8 && ky == -player_move+4)
                {
                    v = find_fig(kx,ky);
                    if (v[0] != player_move)
                    {
                        if (b || find_in_close(kx,ky))
                        {
                            if (v[0] == -1)
                            {
                                w.x = kx;
                                w.y = ky;
                                moves.push_back(w);
                                if (domarks)
                                    marks[kx][ky] = 1;
                            }
                        }
                    }
                }
            }
        }
        if (f.get_state() == 'k')
        {
            int x = f.get_x(), y = f.get_y();
            char s = f.get_state();
            f.eaten();
            attacking(1-player_move,0);
            f.rejoin(x,y,s);
            for (int p=0;p<8;p++)
            {
                kx = (f.get_x()-4)/100+Xways[0][p];
                ky = (f.get_y()-4)/100+Xways[1][p];
                if (kx>=0 && kx<8 && ky>=0 && ky<8 && attack[kx][ky]!=1)
                {
                    v = find_fig(kx,ky);
                    if (v[0] != player_move)
                        if (v[0] != -1)
                        {
                            w.x = kx;
                            w.y = ky;
                            moves.push_back(w);
                            if (domarks)
                                marks[kx][ky] = 2;
                        }
                        else
                        {
                            w.x = kx;
                            w.y = ky;
                            moves.push_back(w);
                            if (domarks)
                                marks[kx][ky] = 1;
                        }
                }
            }
            if (!king[player_move])
            {
                for (int p=0;p<2;p++)
                {
                    kx = (f.get_x()-4)/100;
                    ky = (f.get_y()-4)/100;
                    while (true)
                    {
                        if (kx>=0 && kx<8 && ky>=0 && ky<8)
                        {
                            v = find_fig(kx,ky);
                            if (v[0] != -1)
                                if (figures[v[0]][v[1]].get_state() == 'r' && v[0] == player_move && !rook[2*player_move+p])
                                {
                                    w.x = (f.get_x()-4)/100-4*p+2;
                                    w.y = (f.get_y()-4)/100;
                                    moves.push_back(w);
                                    if (domarks)
                                        marks[w.x][w.y] = 1;
                                    break;
                                }
                            if (attack[kx][ky] == 1)
                                break;
                            if (kx == (f.get_x()-4)/100)
                            {
                                kx-=2*p-1;
                                continue;
                            }
                            if (v[0] != -1)
                                break;
                            else
                                kx-=2*p-1;
                        }
                        else
                            break;
                    }
                }
            }
        }
    }
    bool find_in_moves(int kx, int ky)
    {
        for (int i=0;i<moves.size();i++)
            if (moves[i].x == kx && moves[i].y == ky)
                return 1;
        return 0;
    }
    bool stalemate(int player_move)
    {
        for (int i=0;i<figures[player_move].size();i++)
        {
            possible_moves(figures[player_move][i],0,player_move);
            if (moves.size()>1)
                return 0;
        }
        return 1;
    }
    bool check(int player_move)
    {
        attacking(1-player_move,0);
        if (attack[(figures[player_move][find_king(player_move)].get_x()-4)/100][(figures[player_move][find_king(player_move)].get_y()-4)/100] == 1)
            return 1;
        else
            return 0;
    }
    int game_end(int player_move)
    {
        pos_to_fen(1-player_move);
        if (rule50 == 100)
        {
            game_over.setString("Draw: 50 moves rule");
            game_state = -1;
            return -2;
        }
        if (repetition == 2)
        {
            game_over.setString("Draw: repetition");
            game_state = -1;
            return -3;
        }
        if (stalemate(player_move))
            if (!check(player_move))
            {
                game_over.setString("Draw: stalemate");
                game_state = -1;
                return -1;
            }
            else
            {
                if (player_move == 1)
                    game_over.setString("White won: ");
                else
                    game_over.setString("Black won: ");
                game_over.setString(game_over.getString() + "mate");
                game_state = -1;
                return 2;
            }
        else
            if (check(player_move))
                return 1;
        return 0;
    }
    void draw_marks(RenderWindow& app)
    {
        for (int i=0;i<8;i++)
            for (int j=0;j<8;j++)
                if (marks[j][i]==1)
                {
                    spr_dot.setPosition(100*j+4,100*i+4);
                    app.draw(spr_dot);
                }
                else
                    if (marks[j][i]==2)
                    {
                        spr_mark.setPosition(100*j+4,100*i+4);
                        app.draw(spr_mark);
                    }
                    else
                        if (marks[j][i]==3)
                        {
                            spr_last.setPosition(100*j+4,100*i+4);
                            app.draw(spr_last);
                        }
    }
    bool illegal_move(int x, int y)
    {
        for (int i=0;i<moves.size();i++)
            if (moves[i].x == x && moves[i].y == y)
                return 0;
        return 1;
    }
    void enpassanting(Vector2i pos,int player_move, int fig_move, int fig_x, int fig_y)
    {
        if (figures[player_move][fig_move].get_state() == 'p')
        {
            if (enpassant.x == (pos.x-3)/100 && enpassant.y == (pos.y-3)/100)
            {
                std::vector <int> d = find_fig((pos.x-3)/100,(fig_y-4)/100,1-player_move);
                figures[d[0]][d[1]].eaten();
            }
            if (abs((pos.y-3)/100-(fig_y-4)/100) == 2)
            {
                enpassant.x = (pos.x-3)/100;
                enpassant.y = (pos.y-3)/100-2*player_move+1;
            }
            else
                enpassant.x = -1;
        }
        else
            enpassant.x = -1;
    }
    void castling(Vector2i pos,int player_move, int fig_move, int fig_x, int fig_y)
    {
        std::vector <int> g;
        int kx, ky;
        for (int i=0;i<figures.size();i++)
        {
            kx = 4;
            ky = 7*(1-i);
            g = find_fig(kx,ky);                                                //king[0] = KQ
            if (g[0]==-1 || figures[g[0]][g[1]].get_state() != 'k')             //king[1] = kq
                king[i] = 1;
        }
        for (int i=0;i<figures.size();i++)
            for (int p=0;p<2;p++)
            {
                kx = 7*(1-p);                                                   //rook[0] = K
                ky = 7*(1-i);                                                   //rook[1] = Q
                g = find_fig(kx,ky);                                            //rook[2] = k
                if (g[0]==-1 || figures[g[0]][g[1]].get_state() != 'r')         //rook[3] = q
                    rook[2*i+p] = 1;
            }
        if (figures[player_move][fig_move].get_state() == 'k')
        {
            if ((fig_x-4)/100 == (pos.x-3)/100-2 && (fig_y-4)/100 == (pos.y-3)/100)
            {
                g = find_fig((pos.x-3)/100+1,(pos.y-4)/100,player_move);
                figures[player_move][g[1]].rejoin(100*((pos.x-3)/100-1)+4,100*((pos.y-3)/100)+4,'r');
            }
            if ((fig_x-4)/100 == (pos.x-3)/100+2 && (fig_y-4)/100 == (pos.y-3)/100)
            {
                g = find_fig((pos.x-3)/100-2,(pos.y-4)/100,player_move);
                figures[player_move][g[1]].rejoin(100*((pos.x-3)/100+1)+4,100*((pos.y-3)/100)+4,'r');
            }
        }
    }
    void write(RenderWindow& app, Fig& f, int fig_x, int fig_y, Vector2i pos, int player_move, int z)
    {
        std::string s;
        if (f.get_state() != 'p' && !promoted)
        {
            if (f.get_state() == 'k' && (fig_x-4)/100 == (pos.x-3)/100-2 && (fig_y-4)/100 == (pos.y-3)/100)
                s+="0-0";
            else
                if (f.get_state() == 'k' && (fig_x-4)/100 == (pos.x-3)/100+2 && (fig_y-4)/100 == (pos.y-3)/100)
                    s+="0-0-0";
                else
                {
                    s+=std::toupper(f.get_state());
                    int cx = f.get_x(), cy = f.get_y();
                    f.rejoin(fig_x,fig_y,f.get_state());
                    char s1 = ' ', s2 = ' ';
                    for (int i=0;i<figures[player_move].size();i++)
                    {
                        if (figures[player_move][i].get_state() != f.get_state() || (figures[player_move][i].get_x() == f.get_x() && figures[player_move][i].get_y() == f.get_y()))
                            continue;
                        possible_moves(figures[player_move][i],0,player_move);
                        if (!illegal_move((pos.x-3)/100,(pos.y-3)/100) && figures[player_move][i].get_x() != fig_x)
                            s1=char((fig_x-4)/100+'a');
                        if (!illegal_move((pos.x-3)/100,(pos.y-3)/100) && figures[player_move][i].get_x() == fig_x)
                            s2=char('0'+8-(fig_y-4)/100);
                    }
                    if (s1 != ' ')
                        s+=s1;
                    if (s2 != ' ')
                        s+=s2;
                    f.rejoin(cx,cy,f.get_state());
                    if (marks[(pos.x-3)/100][(pos.y-3)/100] == 2)
                        s+='x';
                    s+=char((pos.x-3)/100+'a');
                    s+=char('0'+8-(pos.y-3)/100);
                }
        }
        else
        {
            s+=char((fig_x-4)/100+'a');
            if (marks[(pos.x-3)/100][(pos.y-3)/100] == 2)
            {
                s+='x';
                s+=char((pos.x-3)/100+'a');
            }
            s+=char('0'+8-(pos.y-3)/100);
            if (promoted)
            {
                s+='=';
                s+=std::toupper(f.get_state());
            }
        }
        if (z == 1)
            s+='+';
        else
            if (z == 2)
                s+='#';
        Text l;
        l.setFont(font);
        l.setString(s);
        l.setCharacterSize(24);
        l.setColor(Color(0,0,0));
        if (!from_editor)
        {
            if (n1.moves.size() > 1)
                l.setPosition(n1.moves[n1.moves.size()-2].getPosition().x,n1.moves[n1.moves.size()-2].getPosition().y+30);
            else
                l.setPosition(930+130*(n1.moves.size()%2),35);
            n1.rect.setPosition(l.getPosition());
            n1.moves.push_back(l);
            if (n1.moves.size()%2 != 0)
            {
                s = std::to_string(((n1.moves.size()-1)/2)+1);
                s+='.';
                l.setString(s);
                if (n1.num.size() != 0)
                    l.setPosition(n1.num[n1.num.size()-1].getPosition().x,n1.num[n1.num.size()-1].getPosition().y+30);
                else
                    l.setPosition(875,35);
                n1.num.push_back(l);
            }
        }
        else
        {
            if (n1.moves.size() > 1)
                l.setPosition(n1.moves[n1.moves.size()-2].getPosition().x,n1.moves[n1.moves.size()-2].getPosition().y+30);
            else
                l.setPosition(930+130*((n1.moves.size()+1)%2),35+30*n1.moves.size());
            n1.rect.setPosition(l.getPosition());
            n1.moves.push_back(l);
            l.setString(s);
            if (n1.num.size() == 0)
            {
                s = "1.";
                l.setString(s);
                l.setPosition(875,35);
                n1.num.push_back(l);
            }
            else
                if ((n1.moves.size()+1)%2 != 0)
                {
                    s = std::to_string((n1.moves.size()/2)+1);
                    s+='.';
                    l.setString(s);
                    l.setPosition(n1.num[n1.num.size()-1].getPosition().x,n1.num[n1.num.size()-1].getPosition().y+30);
                    n1.num.push_back(l);
                }
        }
        if (n1.moves[n1.moves.size()-1].getPosition().y > 778)
            move_notes(778-n1.moves[n1.moves.size()-1].getPosition().y-2,0);
    }
    void pos_to_fen(int player_move)
    {
        std::vector <std::vector <char>> x(8,std::vector <char> (8,'0'));
        for (int i=0;i<figures[0].size();i++)
            if (figures[0][i].get_state()!= '0')
                x[(figures[0][i].get_x()-4)/100][(figures[0][i].get_y()-4)/100] = std::toupper(figures[0][i].get_state());
        for (int i=0;i<figures[1].size();i++)
            if (figures[1][i].get_state()!= '0')
                x[(figures[1][i].get_x()-4)/100][(figures[1][i].get_y()-4)/100] = figures[1][i].get_state();
        std::string s;
        int k;
        for (int i=0;i<8;i++)
        {
            k=0;
            for (int j=0;j<8;j++)
            {
                if (x[j][i]=='0')
                {
                    k++;
                    if (j == 7)
                       s+=char('0'+k);
                }
                else
                    if (k==0)
                        s+=x[j][i];
                    else
                    {
                        s+=char('0'+k);
                        s+=x[j][i];
                        k=0;
                    }

            }
            s+='/';
        }
        s.pop_back();
        s+=' ';
        if (player_move == 1)
            s+='w';
        else
            s+='b';
        s+=' ';
        if (!king[0])
        {
            if (!rook[0])
                s+='K';
            if (!rook[1])
                s+='Q';
        }
        if (!king[1])
        {
            if (!rook[2])
                s+='k';
            if (!rook[3])
                s+='q';
        }
        if (s[s.size()-1] == ' ')
            s+='-';
        s+=' ';
        if (enpassant.x == -1)
            s+='-';
        else
        {
            for (int i=0;i<figures[1-player_move].size();i++)
                if (figures[1-player_move][i].get_state() == 'p')
                {
                    possible_moves(figures[1-player_move][i],0,1-player_move);
                    if (find_in_moves(enpassant.x,enpassant.y))
                    {
                        s+=char(enpassant.x+'a');
                        s+=char('0'+8-enpassant.y);
                        break;
                    }
                }
            if (s[s.size()-1] == ' ')
                s+='-';
        }
        s+=' ';
        s+=std::to_string(rule50);
        s+=' ';
        s+=std::to_string(n1.num.size()+1);
        repetition = same_pos(s);
        fen.push_back(s);
        fen_i++;
        show_fen[1].setString(s);
    }
    int same_pos(std::string s)
    {
        bool b;
        int k,n = 0;
        for (int i=0;i<fen.size();i++)
        {
            k=0;
            b=0;
            while (s[k]!=' ' || fen[i][k]!=' ')
            {
                if (s[k]!=fen[i][k])
                {
                    b=1;
                    break;
                }
                k++;
            }
            if (b==0)
            {
                n++;
            }
        }
        return n;
    }
    void fen_to_pos(std::string f)
    {
        std::vector <std::vector <Fig>> figs(2);
        int j=0, kx = 0, ky = 0;
        while (f[j]!=' ')
        {
            if (f[j] == 'Q')
                figs[0].push_back(Fig(img_wqueen,tex_wqueen,spr_wqueen,'q',kx,ky));
            else
                if (f[j] == 'R')
                    figs[0].push_back(Fig(img_wrook,tex_wrook,spr_wrook,'r',kx,ky));
                else
                    if (f[j] == 'B')
                        figs[0].push_back(Fig(img_wbishop,tex_wbishop,spr_wbishop,'b',kx,ky));
                    else
                        if (f[j] == 'N')
                            figs[0].push_back(Fig(img_wknight,tex_wknight,spr_wknight,'n',kx,ky));
                        else
                            if (f[j] == 'P')
                                figs[0].push_back(Fig(img_wpawn,tex_wpawn,spr_wpawn,'p',kx,ky));
                            else
                                if (f[j] == 'K')
                                    figs[0].push_back(Fig(img_wking,tex_wking,spr_wking,'k',kx,ky));
                                else
                                    if (f[j] == 'q')
                                        figs[1].push_back(Fig(img_bqueen,tex_bqueen,spr_bqueen,'q',kx,ky));
                                    else
                                        if (f[j] == 'r')
                                            figs[1].push_back(Fig(img_brook,tex_brook,spr_brook,'r',kx,ky));
                                        else
                                            if (f[j] == 'b')
                                                figs[1].push_back(Fig(img_bbishop,tex_bbishop,spr_bbishop,'b',kx,ky));
                                            else
                                                if (f[j] == 'n')
                                                    figs[1].push_back(Fig(img_bknight,tex_bknight,spr_bknight,'n',kx,ky));
                                                else
                                                    if (f[j] == 'p')
                                                        figs[1].push_back(Fig(img_bpawn,tex_bpawn,spr_bpawn,'p',kx,ky));
                                                    else
                                                        if (f[j] == 'k')
                                                            figs[1].push_back(Fig(img_bking,tex_bking,spr_bking,'k',kx,ky));
            if (f[j]-'0' >= 0 && f[j]-'0' <= 8)
                kx+=f[j]-'0';
            else
                if (f[j]=='/')
                {
                    kx = 0;
                    ky++;
                }
                else
                    kx++;
            j++;
        }
        figures = figs;
        j++;
        if (f[j] == 'w')
            player_move = 0;
        else
            player_move = 1;
        j+=2;
        rook[0] = 1; rook[1] = 1; rook[2] = 1; rook[3] = 1;
        king[0] = 1; king[1] = 1;
        while (f[j]!=' ')
        {
            if (f[j] == 'K')
                rook[0] = 0;
            if (f[j] == 'Q')
                rook[1] = 0;
            if (f[j] == 'k')
                rook[2] = 0;
            if (f[j] == 'q')
                rook[3] = 0;
            if (f[j] == 'K' || f[j] == 'Q')
                king[0] = 0;
            if (f[j] == 'k' || f[j] == 'q')
                king[1] = 0;
            j++;
        }
        j++;
        if (f[j] != '-')
        {
            enpassant.x = int(f[j]-'a');
            j++;
            enpassant.y = int(8-f[j]+'0');
        }
        else
            enpassant.x = -1;
        j+=2;
        std::string s;
        while (f[j]!=' ')
        {
            s+=f[j];
            j++;
        }
        rule50 = std::stoi(s);
    }
    void do_promotion(RenderWindow& app, int fig_x, int fig_y, int& player_move, char c)
    {
        figures[player_move][promote_move].promotion(c,player_move);
        game_state = 2;
        player_move = 1-player_move;
        int z = game_end(player_move);
        promoted = 1;
        Vector2i promote_xy;
        promote_xy.x = promote_x;
        promote_xy.y = promote_y;
        write(app,figures[1-player_move][promote_move],fig_x,fig_y,promote_xy,1-player_move,z);
        promoted = 0;
        rule50 = 0;
    }
    void move_notes(int y, bool check)
    {
        if (n1.moves.size()>1 && (n1.moves[0].getPosition().y <= 23 || y<0 || !check) && (n1.moves[n1.moves.size()-1].getPosition().y >= 778 || y>0 || !check))
        {
            for (int i=0;i<n1.moves.size();i++)
                n1.moves[i].move(0,y);
            for (int i=0;i<n1.num.size();i++)
                n1.num[i].move(0,y);
            n1.rect.move(0,y);
        }
    }
    void set_pos(int x)
    {
        fen_i=x;
        fen_to_pos(fen[fen_i]);
        if (fen_i-1 >= 0)
            n1.rect.setPosition(n1.moves[fen_i-1].getPosition());
        else
            n1.rect.setPosition(0,0);
        show_fen[1].setString(fen[fen_i]);
        game_over.setString("");
        clock.restart();
    }
    void step_back()
    {
        arrow_add(1);
        fen.pop_back();
        set_pos(fen.size()-1);
        if (fen.size() == 1)
            player_move = 0;
        n1.moves.pop_back();
        if (player_move == 0)
            n1.num.pop_back();
        game_state = 0;
    }
    void new_game(bool editor, int pl_move)
    {
        arrow_add(1);
        std::string s;
        if (editor)
        {
            s = fen[fen.size()-1];
            if (pl_move == 1)
                from_editor = 1;
        }
        else
            s = fen[0];
        fen.clear();
        fen.push_back(s);
        set_pos(0);
        n1.moves.clear();
        n1.num.clear();
        fen_i = 0;
        game_state = 0;
    }
    void arrow_add(bool do_clear)
    {
        if (do_clear)
        {
            arr.clear();
            arr_i = -1;
        }
        Arrow ar;
        ar.body = RectangleShape(Vector2f(100,10));
        ar.body.setFillColor(Color(180,225,30,230));
        ar.body.setPosition(-100,-100);
        ar.arrow.setPointCount(4);
        ar.arrow.setPoint(0,Vector2f(0,0));
        ar.arrow.setPoint(1,Vector2f(-10,30));
        ar.arrow.setPoint(2,Vector2f(50,0));
        ar.arrow.setPoint(3,Vector2f(-10,-30));
        ar.arrow.setFillColor(Color(180,225,30,250));
        ar.arrow.setPosition(-100,-100);
        arr.push_back(ar);
        arr_i++;
    }
    bool arrow_remove(int x1,int y1,int x2,int y2)
    {
        for (int i=0;i<arr.size();i++)
        {
            if (arr[i].x1 == x1 && arr[i].y1 == y1 && arr[i].x2 == x2 && arr[i].y2 == y2)
            {
                arr.erase(arr.begin()+i);
                arr_i--;
                return 1;
            }
        }
        return 0;
    }
    void draw(RenderWindow& app)
    {
        app.draw(spr_board);
        app.draw(noting[1]);
        if (n1.rect.getPosition().y >= 11 && n1.rect.getPosition().y <= 808)
            app.draw(n1.rect);
        for (int i=0;i<n1.moves.size();i++)
            if (n1.moves[i].getPosition().y >= 11 && n1.moves[i].getPosition().y <= 803)
                app.draw(n1.moves[i]);
        for (int i=0;i<n1.num.size();i++)
            if (n1.num[i].getPosition().y >= 11 && n1.num[i].getPosition().y <= 803)
                app.draw(n1.num[i]);
        app.draw(noting[0]);
        app.draw(noting[2]);
        app.draw(noting[3]);
        app.draw(line);
        for (int i=0;i<figures.size();i++)
            for (int j=0;j<figures[i].size();j++)
                if (j != fig_move || player_move != i)
                    figures[i][j].draw(app);
        app.draw(show_fen[0]);
        app.draw(show_fen[1]);
        app.draw(game_over);
        for (int i=0;i<=arr_i;i++)
        {
            app.draw(arr[i].body);
            app.draw(arr[i].arrow);
        }
        if (game_state <= 0)
        {
            if(copy_fen.update(app,0) && get_fen().getString() != "")
                Clipboard::setString(get_fen().getString());
            if(move_back.update(app,1) && fen_i > 0)
                step_back();
            if(restart.update(app,1) && clock.getElapsedTime().asSeconds() >= 0.15)
                new_game(0,0);
            if(ex.update(app,1) && clock.getElapsedTime().asSeconds() >= 0.15)
                stop = 0;
        }
        else
        {
            copy_fen.draw(app);
            move_back.draw(app);
            restart.draw(app);
            ex.draw(app);
        }
    }
    void play(RenderWindow& app, Vector2i pos)
    {
        if (game_state == 0)                       //if we are waiting for a player to move
        {
            if (Mouse::isButtonPressed(Mouse::Right))
            {
                arr[arr_i].x1 = 100*((pos.x-3)/100)+55;
                arr[arr_i].y1 = 100*((pos.y-3)/100)+55;
                arr[arr_i].body.setPosition(arr[arr_i].x1,arr[arr_i].y1);
                game_state = 6;
            }
            else
            {
                if (Keyboard::isKeyPressed(Keyboard::Left) && fen_i != 0 && clock.getElapsedTime().asSeconds() >= 0.15)
                {
                    arrow_add(1);
                    set_pos(fen_i-1);
                    game_state = 5;
                }
                if (Mouse::isButtonPressed(Mouse::Left) && clock.getElapsedTime().asSeconds() >= 0.15)
                    for (int i=0;i<n1.moves.size();i++)
                        if (pos.x > n1.moves[i].getPosition().x && pos.y > n1.moves[i].getPosition().y && pos.x < n1.moves[i].getPosition().x+130 && pos.y < n1.moves[i].getPosition().y+30)
                        {
                            arrow_add(1);
                            set_pos(i+1);
                            game_state = 5;
                            if (fen_i == fen.size()-1)
                                game_state = 0;
                        }
                if (Mouse::isButtonPressed(Mouse::Left))
                    for (int i=0;i<figures[player_move].size();i++)
                        if ((figures[player_move][i].get_x()-4)/100==(pos.x-3)/100 && (figures[player_move][i].get_y()-4)/100==(pos.y-3)/100)
                        {
                            fig_move = i;
                            fig_x = figures[player_move][i].get_x();
                            fig_y = figures[player_move][i].get_y();
                            possible_moves(figures[player_move][i],1,player_move);
                            game_state = 1;
                            move_type = 0;
                            arrow_add(1);
                            break;
                        }
            }
        }
        if (game_state == 1)                                    //if the figure is held
        {
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                if (move_type == 2)
                {
                    if (illegal_move((pos.x-3)/100,(pos.y-3)/100))
                    {
                        draw_marks(app);
                        figures[player_move][fig_move].follow_mouse(pos);
                        figures[player_move][fig_move].draw(app);
                    }
                    else
                    {
                        v = find_fig((pos.x-3)/100,(pos.y-3)/100,1-player_move);
                        if (v[0]!=-1)
                        {
                            eated = figures[v[0]][v[1]].get_state();
                            figures[v[0]][v[1]].eaten();
                            rule50 = 0;
                        }
                        figures[player_move][fig_move].release(pos.x,pos.y);
                        if ((fig_x-4)/100 != (pos.x-3)/100 || (fig_y-4)/100 != (pos.y-3)/100)
                        {
                            if (figures[player_move][fig_move].get_state() == 'p')
                                rule50 = 0;
                            enpassanting(pos,player_move,fig_move,fig_x,fig_y);
                            castling(pos,player_move,fig_move,fig_x,fig_y);
                        }
                        figures[player_move][fig_move].draw(app);
                        promote_move = fig_move;
                        fig_move = -1;
                        game_state = 2;
                        if (figures[player_move][promote_move].get_state() == 'p' && (pos.y-3)/100 == 7*player_move)
                        {
                            game_state = 3;
                            promote_x = 100*((pos.x-3)/100)+4;
                            promote_y = 100*((pos.y-3)/100)+4;
                            if (player_move == 1)
                                promotescreen_y = promote_y-301;
                            else
                                promotescreen_y = promote_y-1;
                        }
                        else
                        {
                            if ((fig_x-4)/100 != (pos.x-3)/100 || (fig_y-4)/100 != (pos.y-3)/100)
                            {
                                player_move = 1-player_move;
                                int z = game_end(player_move);
                                write(app,figures[1-player_move][promote_move],fig_x,fig_y,pos,1-player_move,z);
                                rule50++;
                            }
                        }
                    }
                }
                else
                {
                    draw_marks(app);
                    figures[player_move][fig_move].follow_mouse(pos);
                    figures[player_move][fig_move].draw(app);
                    if ((fig_x-4)/100 != (pos.x-3)/100 || (fig_y-4)/100 != (pos.y-3)/100)
                        move_type = 1;
                }
            }
            else
            {
                if (move_type == 1)
                {
                    if (illegal_move((pos.x-3)/100,(pos.y-3)/100))
                    {
                        figures[player_move][fig_move].release(fig_x-1,fig_y-1);
                        figures[player_move][fig_move].draw(app);
                        fig_move = -1;
                        game_state = 0;
                    }
                    else
                    {
                        v = find_fig((pos.x-3)/100,(pos.y-3)/100,1-player_move);
                        if (v[0]!=-1)
                        {
                            eated = figures[v[0]][v[1]].get_state();
                            figures[v[0]][v[1]].eaten();
                            rule50 = 0;
                        }
                        figures[player_move][fig_move].release(pos.x,pos.y);
                        if ((fig_x-4)/100 != (pos.x-3)/100 || (fig_y-4)/100 != (pos.y-3)/100)
                        {
                            if (figures[player_move][fig_move].get_state() == 'p')
                                rule50 = 0;
                            enpassanting(pos,player_move,fig_move,fig_x,fig_y);
                            castling(pos,player_move,fig_move,fig_x,fig_y);
                        }
                        figures[player_move][fig_move].draw(app);
                        promote_move = fig_move;
                        fig_move = -1;
                        game_state = 0;
                        if (figures[player_move][promote_move].get_state() == 'p' && (pos.y-3)/100 == 7*player_move)
                        {
                            game_state = 3;
                            promote_x = 100*((pos.x-3)/100)+4;
                            promote_y = 100*((pos.y-3)/100)+4;
                            if (player_move == 1)
                                promotescreen_y = promote_y-301;
                            else
                                promotescreen_y = promote_y-1;
                        }
                        else
                        {
                            if ((fig_x-4)/100 != (pos.x-3)/100 || (fig_y-4)/100 != (pos.y-3)/100)
                            {
                                player_move = 1-player_move;
                                int z = game_end(player_move);
                                write(app,figures[1-player_move][promote_move],fig_x,fig_y,pos,1-player_move,z);
                                rule50++;
                            }
                        }
                    }
                }
                else
                {
                    draw_marks(app);
                    figures[player_move][fig_move].follow_mouse(pos);
                    figures[player_move][fig_move].draw(app);
                    move_type = 2;
                }
            }

        }
        if (game_state == 2)             //special case
            if (!Mouse::isButtonPressed(Mouse::Left))
                game_state = 0;
        if (game_state == 3)             //special case
            if (!Mouse::isButtonPressed(Mouse::Left))
                game_state = 4;
        if (game_state == 4)             //promotion
        {
            Sprite spr_promote;
            if (player_move == 0)
                spr_promote = spr_wpromote;
            else
                spr_promote = spr_bpromote;
            spr_promote.setPosition(promote_x,promotescreen_y);
            app.draw(rectangle);
            app.draw(spr_promote);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                if ((pos.x-3)/100 == (promote_x-4)/100 && (pos.y-3)/100 == (promote_y-4)/100)
                    do_promotion(app,fig_x,fig_y,player_move,'q');
                else
                    if ((pos.x-3)/100 == (promote_x-4)/100 && (pos.y-3)/100 == (promote_y-4)/100-2*player_move+1)
                        do_promotion(app,fig_x,fig_y,player_move,'r');
                    else
                        if ((pos.x-3)/100 == (promote_x-4)/100 && (pos.y-3)/100 == (promote_y-4)/100-4*player_move+2)
                            do_promotion(app,fig_x,fig_y,player_move,'b');
                        else
                            if ((pos.x-3)/100 == (promote_x-4)/100 && (pos.y-3)/100 == (promote_y-4)/100-6*player_move+3)
                                do_promotion(app,fig_x,fig_y,player_move,'n');
                            else
                            {
                                figures[player_move][promote_move].rejoin(fig_x,fig_y,'p');
                                game_state = 2;
                                if (v[0]!=-1)
                                    figures[v[0]][v[1]].rejoin(promote_x+1,promote_y+1,eated);
                            }
            }
        }
        if (game_state == 5)                 //viewing
        {
            if (clock.getElapsedTime().asSeconds() >= 0.15)
            {
                if (Keyboard::isKeyPressed(Keyboard::Left) && fen_i != 0)
                    set_pos(fen_i-1);
                else
                    if (Keyboard::isKeyPressed(Keyboard::Right))
                    {
                        set_pos(fen_i+1);
                        if (fen_i == fen.size()-1)
                            game_state = 0;
                    }
                if (Mouse::isButtonPressed(Mouse::Left))
                    for (int i=0;i<n1.moves.size();i++)
                        if (pos.x > n1.moves[i].getPosition().x && pos.y > n1.moves[i].getPosition().y && pos.x < n1.moves[i].getPosition().x+130 && pos.y < n1.moves[i].getPosition().y+30)
                        {
                            set_pos(i+1);
                            if (fen_i == fen.size()-1)
                                game_state = 0;
                        }
            }
        }
        if (game_state == 6)                 //draw an arrow
        {
            if (Mouse::isButtonPressed(Mouse::Right))
            {
                double deg = atan((pos.y-arr[arr_i].y1)/double(pos.x-arr[arr_i].x1));
                double z = std::sqrt((pos.y-arr[arr_i].y1)*(pos.y-arr[arr_i].y1)+(pos.x-arr[arr_i].x1)*(pos.x-arr[arr_i].x1))-10;
                if (z>0)
                    arr[arr_i].body.setSize(Vector2f(std::sqrt((pos.y-arr[arr_i].y1)*(pos.y-arr[arr_i].y1)+(pos.x-arr[arr_i].x1)*(pos.x-arr[arr_i].x1))-10,10));
                else
                    arr[arr_i].body.setSize(Vector2f(0,10));
                if (pos.x-arr[arr_i].x1 > 0)
                {
                    arr[arr_i].body.setRotation(180*deg/3.1415);
                    arr[arr_i].body.setPosition(arr[arr_i].x1+5*std::sin(deg),arr[arr_i].y1-5*std::cos(deg));
                    arr[arr_i].arrow.setRotation(180*deg/3.1415);
                    arr[arr_i].arrow.setPosition(pos.x-50*std::cos(deg),pos.y-50*std::sin(deg));
                }
                if (pos.x-arr[arr_i].x1 < 0)
                {
                    arr[arr_i].body.setRotation(180+180*deg/3.1415);
                    arr[arr_i].body.setPosition(arr[arr_i].x1-5*std::sin(deg),arr[arr_i].y1+5*std::cos(deg));
                    arr[arr_i].arrow.setRotation(180+180*deg/3.1415);
                    arr[arr_i].arrow.setPosition(pos.x+50*std::cos(deg),pos.y+50*std::sin(deg));
                }
            }
            else
            {
                if (100*((pos.x-3)/100)+55==arr[arr_i].x1 && 100*((pos.y-3)/100)+55==arr[arr_i].y1)
                {
                    arr[arr_i].body.setPosition(-100,-100);
                    arr[arr_i].arrow.setPosition(-100,-100);
                }
                else
                    if (arrow_remove(arr[arr_i].x1,arr[arr_i].y1,100*((pos.x-3)/100)+55,100*((pos.y-3)/100)+55))
                    {
                        arr[arr_i].body.setPosition(-100,-100);
                        arr[arr_i].arrow.setPosition(-100,-100);
                    }
                    else
                    {
                        arr[arr_i].x2 = 100*((pos.x-3)/100)+55;
                        arr[arr_i].y2 = 100*((pos.y-3)/100)+55;
                        double deg = atan((100*((pos.y-3)/100)+55-arr[arr_i].y1)/double(100*((pos.x-3)/100)+55-arr[arr_i].x1));
                        arr[arr_i].body.setSize(Vector2f(std::sqrt((100*((pos.y-3)/100)+55-arr[arr_i].y1)*(100*((pos.y-3)/100)+55-arr[arr_i].y1)+(100*((pos.x-3)/100)+55-arr[arr_i].x1)*(100*((pos.x-3)/100)+55-arr[arr_i].x1))-10,10));
                        if (100*((pos.x-3)/100)+54-arr[arr_i].x1 > 0)
                        {
                            arr[arr_i].body.setRotation(180*deg/3.1415);
                            arr[arr_i].body.setPosition(arr[arr_i].x1+5*std::sin(deg),arr[arr_i].y1-5*std::cos(deg));
                            arr[arr_i].arrow.setRotation(180*deg/3.1415);
                            arr[arr_i].arrow.setPosition(100*((pos.x-3)/100)+55-50*std::cos(deg),100*((pos.y-3)/100)+55-50*std::sin(deg));
                        }
                        if (100*((pos.x-3)/100)+55-arr[arr_i].x1 < 0)
                        {
                            arr[arr_i].body.setRotation(180+180*deg/3.1415);
                            arr[arr_i].body.setPosition(arr[arr_i].x1-5*std::sin(deg),arr[arr_i].y1+5*std::cos(deg));
                            arr[arr_i].arrow.setRotation(180+180*deg/3.1415);
                            arr[arr_i].arrow.setPosition(100*((pos.x-3)/100)+55+50*std::cos(deg),100*((pos.y-3)/100)+55+50*std::sin(deg));
                        }
                        if (100*((pos.x-3)/100)+55-arr[arr_i].x1 == 0)
                        {
                            deg = (100*((pos.y-3)/100)+55-arr[arr_i].y1)/std::abs(double(100*((pos.y-3)/100)+55-arr[arr_i].y1));
                            arr[arr_i].body.setRotation(90*deg);
                            arr[arr_i].body.setPosition(arr[arr_i].x1+5*deg,arr[arr_i].y1);
                            arr[arr_i].arrow.setRotation(90*deg);
                            arr[arr_i].arrow.setPosition(100*((pos.x-3)/100)+55,100*((pos.y-3)/100)+55-50*deg);
                        }
                        arrow_add(0);
                    }
                game_state = 0;
            }
        }
    }
    Text get_fen() {return show_fen[1];}
    int get_player_move() {return player_move;}
    int stop = -1;
private:
    Image img_dot, img_mark, img_last, img_board, img_wpromote, img_bpromote;
    Texture tex_dot, tex_mark, tex_last, tex_board, tex_wpromote, tex_bpromote;
    Sprite spr_dot, spr_mark, spr_last, spr_board, spr_wpromote, spr_bpromote;
    RectangleShape rectangle;
    std::vector <Arrow> arr;
    int arr_i = 0;
    std::vector <std::vector <Fig>> figures;
    std::vector <std::vector <int>> attack;
    std::vector <std::vector <int>> marks;
    std::vector <Vector2i> moves;
    std::vector <Vector2i> close;
    std::vector <std::string> fen;
    int player_move = 0 ,game_state = 0, fig_x, fig_y, fig_move = -1, move_type = 0, rule50 = 0, repetition = 0, fen_i = -1;
    int promote_x, promote_y, promote_move, promotescreen_y;
    std::vector <std::vector <int>> Xways;
    std::vector <std::vector <int>> Yways;
    std::vector <bool> king;
    std::vector <bool> rook;
    Vector2i enpassant;
    Notes n1;
    Text line, game_over, show_fen[2];
    std::vector <RectangleShape> noting;
    Button copy_fen, move_back, restart, ex;
    bool promoted = 0, from_editor = 0;
    char eated;
    std::vector <int> v;
    Clock clock;
    Image img_wqueen, img_wrook, img_wbishop, img_wknight, img_wpawn, img_wking, img_bqueen, img_brook, img_bbishop, img_bknight, img_bpawn, img_bking;
    Texture tex_wqueen, tex_wrook, tex_wbishop, tex_wknight, tex_wpawn, tex_wking, tex_bqueen, tex_brook, tex_bbishop, tex_bknight, tex_bpawn, tex_bking;
    Sprite spr_wqueen, spr_wrook, spr_wbishop, spr_wknight, spr_wpawn, spr_wking, spr_bqueen, spr_brook, spr_bbishop, spr_bknight, spr_bpawn, spr_bking;
};

class Editor_board
{
public:
    Editor_board(std::vector <std::vector <Fig>> x)
    {
        figures = x;
        load();

        rectangle = RectangleShape(Vector2f(800,800));
        rectangle.setFillColor(Color(255,255,255,128));
        rectangle.setPosition(3,3);
        mode = RectangleShape(Vector2f(98,98));
        mode.setFillColor(Color(0,0,0,32));
        mode.setPosition(-100,-100);

        show_fen[0].setFont(font);
        show_fen[0].setString("FEN: ");
        show_fen[0].setPosition(67,850);
        show_fen[0].setCharacterSize(18);
        show_fen[0].setColor(Color(0,0,0));
        show_fen[1].setFont(font);
        show_fen[1].setPosition(110,850);
        show_fen[1].setCharacterSize(18);
        show_fen[1].setColor(Color(0,0,0));

        copy_fen = Button (3,850,60,22,"copy");
        restart = Button (963,850,100,22,"clear");
        play_pos = Button (1073,850,100,22,"play");
        ex = Button (1127,3,60,27,"exit");
        castle.push_back(Switch(877,600,img_check,tex_check,spr_check,"white castle king side"));
        castle.push_back(Switch(877,635,img_check,tex_check,spr_check,"white castle queen side"));
        castle.push_back(Switch(877,670,img_check,tex_check,spr_check,"black castle king side"));
        castle.push_back(Switch(877,705,img_check,tex_check,spr_check,"black castle queen side"));
        play_color = Switch(877,750,"white to move");

        Xways = std::vector <std::vector <int>> (2);
        Yways = std::vector <std::vector <int>> (2);
        Xways[0] = {1,-1,1,-1,1,-1,0,0};
        Xways[1] = {-1,-1,1,1,0,0,1,-1};
        Yways[0] = {1,1,-1,-1,2,2,-2,-2};
        Yways[1] = {2,-2,2,-2,1,-1,1,-1};

        attack = std::vector <std::vector <int>> (8,std::vector <int> (8,0));
        do_castle = std::vector <bool> (4,0);

        mouse_fig = Fig();
    }
    void load()
    {
        img_board.loadFromFile("img/board.bmp");
        tex_board.loadFromImage(img_board);
        spr_board.setTexture(tex_board);

        img_create.loadFromFile("img/create.bmp");
        tex_create.loadFromImage(img_create);
        spr_create.setTexture(tex_create);
        spr_create.setPosition(875,3);

        img_check.loadFromFile("img/check.bmp");
        img_check.createMaskFromColor(Color(255,255,255));
        tex_check.loadFromImage(img_check);
        spr_check.setTexture(tex_check);

        img_wqueen.loadFromFile("img/wqueen.bmp");
        img_wqueen.createMaskFromColor(Color(255,255,255));
        tex_wqueen.loadFromImage(img_wqueen);
        spr_wqueen.setTexture(tex_wqueen);
        img_wrook.loadFromFile("img/wrook.bmp");
        img_wrook.createMaskFromColor(Color(255,255,255));
        tex_wrook.loadFromImage(img_wrook);
        spr_wrook.setTexture(tex_wrook);
        img_wbishop.loadFromFile("img/wbishop.bmp");
        img_wbishop.createMaskFromColor(Color(255,255,255));
        tex_wbishop.loadFromImage(img_wbishop);
        spr_wbishop.setTexture(tex_wbishop);
        img_wknight.loadFromFile("img/wknight.bmp");
        img_wknight.createMaskFromColor(Color(255,255,255));
        tex_wknight.loadFromImage(img_wknight);
        spr_wknight.setTexture(tex_wknight);
        img_wpawn.loadFromFile("img/wpawn.bmp");
        img_wpawn.createMaskFromColor(Color(255,255,255));
        tex_wpawn.loadFromImage(img_wpawn);
        spr_wpawn.setTexture(tex_wpawn);
        img_wking.loadFromFile("img/wking.bmp");
        img_wking.createMaskFromColor(Color(255,255,255));
        tex_wking.loadFromImage(img_wking);
        spr_wking.setTexture(tex_wking);
        img_bqueen.loadFromFile("img/bqueen.bmp");
        img_bqueen.createMaskFromColor(Color(255,255,255));
        tex_bqueen.loadFromImage(img_bqueen);
        spr_bqueen.setTexture(tex_bqueen);
        img_brook.loadFromFile("img/brook.bmp");
        img_brook.createMaskFromColor(Color(255,255,255));
        tex_brook.loadFromImage(img_brook);
        spr_brook.setTexture(tex_brook);
        img_bbishop.loadFromFile("img/bbishop.bmp");
        img_bbishop.createMaskFromColor(Color(255,255,255));
        tex_bbishop.loadFromImage(img_bbishop);
        spr_bbishop.setTexture(tex_bbishop);
        img_bknight.loadFromFile("img/bknight.bmp");
        img_bknight.createMaskFromColor(Color(255,255,255));
        tex_bknight.loadFromImage(img_bknight);
        spr_bknight.setTexture(tex_bknight);
        img_bpawn.loadFromFile("img/bpawn.bmp");
        img_bpawn.createMaskFromColor(Color(255,255,255));
        tex_bpawn.loadFromImage(img_bpawn);
        spr_bpawn.setTexture(tex_bpawn);
        img_bking.loadFromFile("img/bking.bmp");
        img_bking.createMaskFromColor(Color(255,255,255));
        tex_bking.loadFromImage(img_bking);
        spr_bking.setTexture(tex_bking);
    }
    std::vector <int> find_fig(int x, int y)
    {
        std::vector <int> v(2,-1);
        for (int i=0;i<2;i++)
            for (int j=0;j<figures[i].size();j++)
                if ((figures[i][j].get_x()-4)/100 == x && (figures[i][j].get_y()-4)/100 == y)
                {
                    v[0] = i;
                    v[1] = j;
                    return v;
                }
        return v;  //empty
    }
    std::vector <int> find_fig(int x, int y,int noi, int noj)
    {
        std::vector <int> v(2,-1);
        for (int i=0;i<figures.size();i++)
            for (int j=0;j<figures[i].size();j++)
            {
                if (i == noi && j == noj)
                    continue;
                if ((figures[i][j].get_x()-4)/100 == x && (figures[i][j].get_y()-4)/100 == y)
                {
                    v[0] = i;
                    v[1] = j;
                    return v;
                }
            }
        return v;  //empty
    }
    void pos_to_fen(int player_move)
    {
        std::vector <std::vector <char>> x(8,std::vector <char> (8,'0'));
        for (int i=0;i<figures[0].size();i++)
            if (figures[0][i].get_state()!= '0')
                x[(figures[0][i].get_x()-4)/100][(figures[0][i].get_y()-4)/100] = std::toupper(figures[0][i].get_state());
        for (int i=0;i<figures[1].size();i++)
            if (figures[1][i].get_state()!= '0')
                x[(figures[1][i].get_x()-4)/100][(figures[1][i].get_y()-4)/100] = figures[1][i].get_state();
        std::string s;
        int k;
        for (int i=0;i<8;i++)
        {
            k=0;
            for (int j=0;j<8;j++)
            {
                if (x[j][i]=='0')
                {
                    k++;
                    if (j == 7)
                       s+=char('0'+k);
                }
                else
                    if (k==0)
                        s+=x[j][i];
                    else
                    {
                        s+=char('0'+k);
                        s+=x[j][i];
                        k=0;
                    }

            }
            s+='/';
        }
        s.pop_back();
        s+=' ';
        if (player_move == 0)
            s+='w';
        else
            s+='b';
        s+=' ';
        if (do_castle[0])
            s+='K';
        if (do_castle[1])
            s+='Q';
        if (do_castle[2])
            s+='k';
        if (do_castle[3])
            s+='q';
        if (s[s.size()-1] == ' ')
            s+='-';
        s+=' ';
        s+='-';
        s+=' ';
        s+='0';
        s+=' ';
        s+='1';
        fen = s;
        show_fen[1].setString(s);
    }
    void fen_to_pos(std::string f)
    {
        std::vector <std::vector <Fig>> figs(2);
        int j=0, kx = 0, ky = 0;
        while (f[j]!=' ')
        {
            if (f[j] == 'Q')
                figs[0].push_back(Fig(img_wqueen,tex_wqueen,spr_wqueen,'q',kx,ky));
            else
                if (f[j] == 'R')
                    figs[0].push_back(Fig(img_wrook,tex_wrook,spr_wrook,'r',kx,ky));
                else
                    if (f[j] == 'B')
                        figs[0].push_back(Fig(img_wbishop,tex_wbishop,spr_wbishop,'b',kx,ky));
                    else
                        if (f[j] == 'N')
                            figs[0].push_back(Fig(img_wknight,tex_wknight,spr_wknight,'n',kx,ky));
                        else
                            if (f[j] == 'P')
                                figs[0].push_back(Fig(img_wpawn,tex_wpawn,spr_wpawn,'p',kx,ky));
                            else
                                if (f[j] == 'K')
                                    figs[0].push_back(Fig(img_wking,tex_wking,spr_wking,'k',kx,ky));
                                else
                                    if (f[j] == 'q')
                                        figs[1].push_back(Fig(img_bqueen,tex_bqueen,spr_bqueen,'q',kx,ky));
                                    else
                                        if (f[j] == 'r')
                                            figs[1].push_back(Fig(img_brook,tex_brook,spr_brook,'r',kx,ky));
                                        else
                                            if (f[j] == 'b')
                                                figs[1].push_back(Fig(img_bbishop,tex_bbishop,spr_bbishop,'b',kx,ky));
                                            else
                                                if (f[j] == 'n')
                                                    figs[1].push_back(Fig(img_bknight,tex_bknight,spr_bknight,'n',kx,ky));
                                                else
                                                    if (f[j] == 'p')
                                                        figs[1].push_back(Fig(img_bpawn,tex_bpawn,spr_bpawn,'p',kx,ky));
                                                    else
                                                        if (f[j] == 'k')
                                                            figs[1].push_back(Fig(img_bking,tex_bking,spr_bking,'k',kx,ky));
            if (f[j]-'0' >= 0 && f[j]-'0' <= 8)
                kx+=f[j]-'0';
            else
                if (f[j]=='/')
                {
                    kx = 0;
                    ky++;
                }
                else
                    kx++;
            j++;
        }
        figures = figs;
    }
    void set_pos(std::string f)
    {
        fen = f;
        fen_to_pos(fen);
        show_fen[1].setString(fen);
        clock.restart();
    }
    void new_game()
    {
        figures.clear();
        figures = std::vector <std::vector <Fig>> (2);
        game_state = 0;
        pos_to_fen(real_player_move);
    }
    void change_mode(Image& img, Texture& tex, Sprite spr, char state, int x, int y, int i)
    {
        mode.setPosition(x,y);
        spr.setScale(0.5,0.5);
        mouse_fig = Fig(img,tex,spr,state,-1,-1);
        mouse_fig_i = i;
    }
    void attacking(int player_move)
    {
        for (int i=0;i<8;i++)
            for (int j=0;j<8;j++)
                attack[j][i] = 0;
        int kx, ky;
        for (int i=0;i<figures[player_move].size();i++)
        {
            if (figures[player_move][i].get_state()=='q')
            {
                for (int p=0;p<8;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    while (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                        if (v[0] != -1)
                            break;
                        kx+=Xways[0][p];
                        ky+=Xways[1][p];
                    }
                }

            }
            if (figures[player_move][i].get_state()=='b')
            {
                for (int p=0;p<4;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    while (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                        if (v[0] != -1)
                            break;
                        kx+=Xways[0][p];
                        ky+=Xways[1][p];
                    }
                }

            }
            if (figures[player_move][i].get_state()=='r')
            {
                for (int p=4;p<8;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    while (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                        if (v[0] != -1)
                            break;
                        kx+=Xways[0][p];
                        ky+=Xways[1][p];
                    }
                }

            }
            if (figures[player_move][i].get_state()=='n')
            {
                for (int p=0;p<8;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Yways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Yways[1][p];
                    if (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                    }
                }

            }
            if (figures[player_move][i].get_state()=='p')
            {
                for (int p=0+2*player_move;p<2+2*player_move;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    if (kx>=0 && kx<8 && ky>=0 && ky<8)
                    {
                        std::vector <int> v = find_fig(kx,ky);
                        attack[kx][ky] = 1;
                    }
                }

            }
            if (figures[player_move][i].get_state()=='k')
            {
                for (int p=0;p<8;p++)
                {
                    kx = (figures[player_move][i].get_x()-4)/100+Xways[0][p];
                    ky = (figures[player_move][i].get_y()-4)/100+Xways[1][p];
                    if (kx>=0 && kx<8 && ky>=0 && ky<8)
                        attack[kx][ky] = 1;
                }

            }
        }
    }
    int find_king(int player_move)
    {
        for (int i=0;i<figures[player_move].size();i++)
            if (figures[player_move][i].get_state() == 'k')
                return i;
    }
    bool check(int player_move)
    {
        attacking(1-player_move);
        if (attack[(figures[player_move][find_king(player_move)].get_x()-4)/100][(figures[player_move][find_king(player_move)].get_y()-4)/100] == 1)
            return 1;
        else
            return 0;
    }
    bool possible_pos(int player_move)
    {
        std::vector <int> king(2,0);
        for (int i=0;i<figures.size();i++)
            for (int j=0;j<figures[i].size();j++)
            {
                if (figures[i][j].get_state() == 'p' && ((figures[i][j].get_y()-4)/100 == 0 || (figures[i][j].get_y()-4)/100 == 7))
                    return 0;
                if (figures[i][j].get_state() == 'k')
                    king[i]++;
            }
        if (king[0] != 1 || king[1] != 1)
            return 0;
        if (check(1-player_move))
            return 0;
        return 1;
    }
    void draw(RenderWindow& app)
    {
        app.draw(spr_board);
        app.draw(spr_create);
        for (int i=0;i<figures.size();i++)
            for (int j=0;j<figures[i].size();j++)
                if (j != fig_move || player_move != i)
                    figures[i][j].draw(app);
        app.draw(show_fen[0]);
        app.draw(show_fen[1]);
        app.draw(mode);
        if (game_state == 0)
        {
            if(copy_fen.update(app,0) && get_fen().getString() != "")
                Clipboard::setString(get_fen().getString());
            if(restart.update(app,1) && clock.getElapsedTime().asSeconds() >= 0.15)
                new_game();
            if(play_pos.update(app,1) && clock.getElapsedTime().asSeconds() >= 0.15)
            {
                if (possible_pos(real_player_move))
                {
                    stop = 1;
                    fen_to_play = fen;
                }
            }
            if(ex.update(app,1) && clock.getElapsedTime().asSeconds() >= 0.15)
                stop = 0;
            for (int i=0;i<4;i++)
                if(castle[i].update(app))
                {
                    do_castle[i] = 1;
                    pos_to_fen(real_player_move);
                }
                else
                {
                    do_castle[i] = 0;
                    pos_to_fen(real_player_move);
                }
            if(play_color.update(app))
            {
                play_color.change_name("black to move");
                real_player_move = 1;
                pos_to_fen(1);
            }
            else
            {
                play_color.change_name("white to move");
                real_player_move = 0;
                pos_to_fen(0);
            }
        }
        else
        {
            copy_fen.draw(app);
            restart.draw(app);
            play_pos.draw(app);
            ex.draw(app);
            for (int i=0;i<4;i++)
                castle[i].draw(app);
            play_color.draw(app);
        }
        mouse_fig.draw(app);
    }
    void play(RenderWindow& app, Vector2i pos)
    {
        mouse_fig_pos = pos;
        mouse_fig_pos.x += 51;
        mouse_fig_pos.y += 51;
        mouse_fig.follow_mouse(mouse_fig_pos);
        if (game_state == 0)                        //if we are waiting for a player to move
        {
            if (Mouse::isButtonPressed(Mouse::Right))   //delete figures
            {
                v = find_fig((pos.x-3)/100,(pos.y-3)/100,-1,-1);
                if (v[0]!=-1)
                {
                    figures[v[0]].erase(figures[v[0]].begin()+v[1]);
                    pos_to_fen(real_player_move);
                }
            }
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                if (pos.x<0 || pos.x>800 || pos.y<0 || pos.y>800)
                {
                    mouse_fig = Fig();
                    mode.setPosition(-100,-100);
                }
                if (mouse_fig.get_state()=='0')
                {
                    for (int i=0;i<figures.size();i++)
                        for (int j=0;j<figures[i].size();j++)
                            if ((figures[i][j].get_x()-4)/100==(pos.x-3)/100 && (figures[i][j].get_y()-4)/100==(pos.y-3)/100)
                            {
                                player_move = i;
                                fig_move = j;
                                fig_x = figures[player_move][j].get_x();
                                fig_y = figures[player_move][j].get_y();
                                game_state = 1;
                                break;
                            }
                }
                else
                {
                    v = find_fig((pos.x-3)/100,(pos.y-3)/100,-1,-1);
                    if (pos.x>0 && pos.x<800 && pos.y>0 && pos.y<800 && v[0] == -1)
                    {
                        if (mouse_fig.get_state() == 'q' && mouse_fig_i == 0)
                            figures[0].push_back(Fig(img_wqueen,tex_wqueen,spr_wqueen,'q',(pos.x-3)/100,(pos.y-3)/100));
                        else
                            if (mouse_fig.get_state() == 'r' && mouse_fig_i == 0)
                                figures[0].push_back(Fig(img_wrook,tex_wrook,spr_wrook,'r',(pos.x-3)/100,(pos.y-3)/100));
                            else
                                if (mouse_fig.get_state() == 'b' && mouse_fig_i == 0)
                                    figures[0].push_back(Fig(img_wbishop,tex_wbishop,spr_wbishop,'b',(pos.x-3)/100,(pos.y-3)/100));
                                else
                                    if (mouse_fig.get_state() == 'n' && mouse_fig_i == 0)
                                        figures[0].push_back(Fig(img_wknight,tex_wknight,spr_wknight,'n',(pos.x-3)/100,(pos.y-3)/100));
                                    else
                                        if (mouse_fig.get_state() == 'p' && mouse_fig_i == 0)
                                            figures[0].push_back(Fig(img_wpawn,tex_wpawn,spr_wpawn,'p',(pos.x-3)/100,(pos.y-3)/100));
                                        else
                                            if (mouse_fig.get_state() == 'k' && mouse_fig_i == 0)
                                                figures[0].push_back(Fig(img_wking,tex_wking,spr_wking,'k',(pos.x-3)/100,(pos.y-3)/100));
                                            else
                                                if (mouse_fig.get_state() == 'q' && mouse_fig_i == 1)
                                                    figures[1].push_back(Fig(img_bqueen,tex_bqueen,spr_bqueen,'q',(pos.x-3)/100,(pos.y-3)/100));
                                                else
                                                    if (mouse_fig.get_state() == 'r' && mouse_fig_i == 1)
                                                        figures[1].push_back(Fig(img_brook,tex_brook,spr_brook,'r',(pos.x-3)/100,(pos.y-3)/100));
                                                    else
                                                        if (mouse_fig.get_state() == 'b' && mouse_fig_i == 1)
                                                            figures[1].push_back(Fig(img_bbishop,tex_bbishop,spr_bbishop,'b',(pos.x-3)/100,(pos.y-3)/100));
                                                        else
                                                            if (mouse_fig.get_state() == 'n' && mouse_fig_i == 1)
                                                                figures[1].push_back(Fig(img_bknight,tex_bknight,spr_bknight,'n',(pos.x-3)/100,(pos.y-3)/100));
                                                            else
                                                                if (mouse_fig.get_state() == 'p' && mouse_fig_i == 1)
                                                                    figures[1].push_back(Fig(img_bpawn,tex_bpawn,spr_bpawn,'p',(pos.x-3)/100,(pos.y-3)/100));
                                                                else
                                                                    if (mouse_fig.get_state() == 'k' && mouse_fig_i == 1)
                                                                        figures[1].push_back(Fig(img_bking,tex_bking,spr_bking,'k',(pos.x-3)/100,(pos.y-3)/100));
                        pos_to_fen(real_player_move);
                    }
                }
               for (int i=0;i<2;i++)
                    for (int j=0;j<6;j++)
                        if (pos.x > 875+98*i && pos.x < 973+98*i && pos.y > 3+98*j && pos.y < 101+98*j)
                        {
                            if (i==0 && j==0)
                                figures[0].push_back(Fig(img_wqueen,tex_wqueen,spr_wqueen,'q',(pos.x-3)/100,(pos.y-3)/100));
                            else
                                if (i==0 && j==1)
                                    figures[0].push_back(Fig(img_wrook,tex_wrook,spr_wrook,'r',(pos.x-3)/100,(pos.y-3)/100));
                                else
                                    if (i==0 && j==2)
                                        figures[0].push_back(Fig(img_wbishop,tex_wbishop,spr_wbishop,'b',(pos.x-3)/100,(pos.y-3)/100));
                                    else
                                        if (i==0 && j==3)
                                            figures[0].push_back(Fig(img_wknight,tex_wknight,spr_wknight,'n',(pos.x-3)/100,(pos.y-3)/100));
                                        else
                                            if (i==0 && j==4)
                                                figures[0].push_back(Fig(img_wpawn,tex_wpawn,spr_wpawn,'p',(pos.x-3)/100,(pos.y-3)/100));
                                            else
                                                if (i==0 && j==5)
                                                    figures[0].push_back(Fig(img_wking,tex_wking,spr_wking,'k',(pos.x-3)/100,(pos.y-3)/100));
                                                else
                                                    if (i==1 && j==0)
                                                        figures[1].push_back(Fig(img_bqueen,tex_bqueen,spr_bqueen,'q',(pos.x-3)/100,(pos.y-3)/100));
                                                    else
                                                        if (i==1 && j==1)
                                                            figures[1].push_back(Fig(img_brook,tex_brook,spr_brook,'r',(pos.x-3)/100,(pos.y-3)/100));
                                                        else
                                                            if (i==1 && j==2)
                                                                figures[1].push_back(Fig(img_bbishop,tex_bbishop,spr_bbishop,'b',(pos.x-3)/100,(pos.y-3)/100));
                                                            else
                                                                if (i==1 && j==3)
                                                                    figures[1].push_back(Fig(img_bknight,tex_bknight,spr_bknight,'n',(pos.x-3)/100,(pos.y-3)/100));
                                                                else
                                                                    if (i==1 && j==4)
                                                                        figures[1].push_back(Fig(img_bpawn,tex_bpawn,spr_bpawn,'p',(pos.x-3)/100,(pos.y-3)/100));
                                                                    else
                                                                        if (i==1 && j==5)
                                                                            figures[1].push_back(Fig(img_bking,tex_bking,spr_bking,'k',(pos.x-3)/100,(pos.y-3)/100));
                            game_state = 1;
                            player_move = i;
                            fig_move = figures[player_move].size()-1;
                            fig_x = -1;
                        }
            }
        }
        if (game_state == 1)             //if the figure is held
        {
            if (!Mouse::isButtonPressed(Mouse::Left))
            {
                if (pos.x<0 || pos.x>800 || pos.y<0 || pos.y>800)
                {
                    for (int i=0;i<2;i++)
                        for (int j=0;j<6;j++)
                            if (pos.x > 875+98*i && pos.x < 973+98*i && pos.y > 3+98*j && pos.y < 101+98*j)
                            {
                                if (i==0 && j==0)
                                    change_mode(img_wqueen,tex_wqueen,spr_wqueen,'q',875+98*i,3+98*j,0);
                                else
                                    if (i==0 && j==1)
                                        change_mode(img_wrook,tex_wrook,spr_wrook,'r',875+98*i,3+98*j,0);
                                    else
                                        if (i==0 && j==2)
                                            change_mode(img_wbishop,tex_wbishop,spr_wbishop,'b',875+98*i,3+98*j,0);
                                        else
                                            if (i==0 && j==3)
                                                change_mode(img_wknight,tex_wknight,spr_wknight,'n',875+98*i,3+98*j,0);
                                            else
                                                if (i==0 && j==4)
                                                    change_mode(img_wpawn,tex_wpawn,spr_wpawn,'p',875+98*i,3+98*j,0);
                                                else
                                                    if (i==0 && j==5)
                                                        change_mode(img_wking,tex_wking,spr_wking,'k',875+98*i,3+98*j,0);
                                                    else
                                                        if (i==1 && j==0)
                                                            change_mode(img_bqueen,tex_bqueen,spr_bqueen,'q',875+98*i,3+98*j,1);
                                                        else
                                                            if (i==1 && j==1)
                                                                change_mode(img_brook,tex_brook,spr_brook,'r',875+98*i,3+98*j,1);
                                                            else
                                                                if (i==1 && j==2)
                                                                    change_mode(img_bbishop,tex_bbishop,spr_bbishop,'b',875+98*i,3+98*j,1);
                                                                else
                                                                    if (i==1 && j==3)
                                                                        change_mode(img_bknight,tex_bknight,spr_bknight,'n',875+98*i,3+98*j,1);
                                                                    else
                                                                        if (i==1 && j==4)
                                                                            change_mode(img_bpawn,tex_bpawn,spr_bpawn,'p',875+98*i,3+98*j,1);
                                                                        else
                                                                            if (i==1 && j==5)
                                                                                change_mode(img_bking,tex_bking,spr_bking,'k',875+98*i,3+98*j,1);
                            }
                    if (fig_x<0)
                        figures[player_move][fig_move].eaten();
                    else
                    {
                        figures[player_move][fig_move].release(fig_x-1,fig_y-1);
                        figures[player_move][fig_move].draw(app);
                    }
                    fig_move = -1;
                    player_move = -1;
                    game_state = 0;
                    pos_to_fen(real_player_move);
                }
                else
                {
                    v = find_fig((pos.x-3)/100,(pos.y-3)/100,player_move,fig_move);
                    if (v[0]!=-1)
                        figures[v[0]][v[1]].eaten();
                    figures[player_move][fig_move].release(pos.x,pos.y);
                    figures[player_move][fig_move].draw(app);
                    fig_move = -1;
                    player_move = -1;
                    game_state = 0;
                    pos_to_fen(real_player_move);
                }
            }
            else
            {
                figures[player_move][fig_move].follow_mouse(pos);
                figures[player_move][fig_move].draw(app);
            }
        }
    }
    Text get_fen() {return show_fen[1];}
    int stop = -1;
    std::string fen_to_play;
private:
    Image img_board, img_create, img_check;
    Texture tex_board, tex_create, tex_check;
    Sprite spr_board, spr_create, spr_check;
    RectangleShape rectangle, mode;
    std::vector <std::vector <Fig>> figures;
    std::string fen;
    int game_state = 0, player_move = -1, fig_move = -1, fig_x, fig_y, mouse_fig_i, real_player_move = 0;
    Text game_over, show_fen[2];
    std::vector <RectangleShape> noting;
    Button copy_fen, restart, play_pos, ex;
    Switch play_color;
    std::vector <Switch> castle;
    std::vector <bool> do_castle;
    std::vector <int> v;
    Fig mouse_fig;
    Vector2i mouse_fig_pos;
    Clock clock;
    std::vector <Vector2i> close;
    std::vector <Vector2i> moves;
    std::vector <std::vector <int>> attack;
    std::vector <std::vector <int>> Xways;
    std::vector <std::vector <int>> Yways;
    Image img_wqueen, img_wrook, img_wbishop, img_wknight, img_wpawn, img_wking, img_bqueen, img_brook, img_bbishop, img_bknight, img_bpawn, img_bking;
    Texture tex_wqueen, tex_wrook, tex_wbishop, tex_wknight, tex_wpawn, tex_wking, tex_bqueen, tex_brook, tex_bbishop, tex_bknight, tex_bpawn, tex_bking;
    Sprite spr_wqueen, spr_wrook, spr_wbishop, spr_wknight, spr_wpawn, spr_wking, spr_bqueen, spr_brook, spr_bbishop, spr_bknight, spr_bpawn, spr_bking;
};

int main()
{
    RenderWindow app(VideoMode(1200, 910), "SFML window");
    app.setFramerateLimit(120);
    font.loadFromFile("font/consola.ttf");
    std::vector <std::vector <Fig>> figs(2);
    for (int i=0;i<16;i++)
        figs[0].push_back(Fig());
    figs[0][0].create("wrook.bmp", 'r', 0, 7);
    figs[0][1].create("wknight.bmp", 'n', 1, 7);
    figs[0][2].create("wbishop.bmp", 'b', 2, 7);
    figs[0][3].create("wqueen.bmp", 'q', 3, 7);
    figs[0][4].create("wking.bmp", 'k', 4, 7);
    figs[0][5].create("wbishop.bmp", 'b', 5, 7);
    figs[0][6].create("wknight.bmp", 'n', 6, 7);
    figs[0][7].create("wrook.bmp", 'r', 7, 7);
    for (int i=0;i<8;i++)
        figs[0][i+8].create("wpawn.bmp", 'p', i, 6);
    for (int i=0;i<16;i++)
        figs[1].push_back(Fig());
    figs[1][0].create("brook.bmp", 'r', 0, 0);
    figs[1][1].create("bknight.bmp", 'n', 1, 0);
    figs[1][2].create("bbishop.bmp", 'b', 2, 0);
    figs[1][3].create("bqueen.bmp", 'q', 3, 0);
    figs[1][4].create("bking.bmp", 'k', 4, 0);
    figs[1][5].create("bbishop.bmp", 'b', 5, 0);
    figs[1][6].create("bknight.bmp", 'n', 6, 0);
    figs[1][7].create("brook.bmp", 'r', 7, 0);
    for (int i=0;i<8;i++)
        figs[1][i+8].create("bpawn.bmp", 'p', i, 1);
    Game_board start(figs);
    Editor_board create(figs);
    Vector2i pos;
    int mode = -1;
    Button play(465,100,150,22,"Play"), editor(465,200,150,22,"Editor");
    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();
        }
        app.clear(Color(255,255,255));
        if (mode == -1)
        {
            if (play.update(app,1))
            {
                start.pos_to_fen(1);
                mode = 1;
            }
            if (editor.update(app,1))
            {
                create.pos_to_fen(1);
                mode = 0;
            }
        }
        if (mode == 1)
        {
            if (event.type == Event::MouseWheelScrolled)
            {
                start.move_notes(-20*event.mouseWheelScroll.delta,1);
                event.mouseWheelScroll.delta = 0;
            }
            start.draw(app);
            pos = Mouse::getPosition(app);
            if (pos.x<0)
                pos.x = 0;
            if (pos.y<0)
                pos.y = 0;
            start.play(app,pos);
            if (start.stop == 0)
            {
                mode = -1;
                start.stop = -1;
            }
        }
        if (mode == 0)
        {
            create.draw(app);
            pos = Mouse::getPosition(app);
            if (pos.x<0)
                pos.x = 0;
            if (pos.y<0)
                pos.y = 0;
            create.play(app,pos);
            if (create.stop == 1)
            {
                start.fen_to_pos(create.fen_to_play);
                start.pos_to_fen(1-start.get_player_move());
                start.new_game(1,start.get_player_move());
                mode = 1;
                create.stop = -1;
            }
            if (create.stop == 0)
            {
                mode = -1;
                create.stop = -1;
            }
        }
        app.display();
    }

    return EXIT_SUCCESS;
}
