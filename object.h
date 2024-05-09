#ifndef OBJECT_H
#define OBJECT_H

#define INTRO_TIMER 1
#define MAIN_MENU_TIMER 2
#define INTRO_IMAGE_TIMER 3
#define GAME_TIMER 4
#define CLOCK_TIMER 5

typedef struct ObjectInfo
{
    int width;
    int height;
    int x;
    int y;
    int dx;
    int dy;
    int type;
    int colorIndex;
    bool isSolid;
    bool isVisible;
    int direction;
} Object;

enum BlockType {NO_SNOW = 1, SNOW, INNER_BLOCK, CANDY, TREE, ENEMY, CHECKPOINT, FOREST, FOREST_INNER_BLOCK, FOREST_TREE, FLOWER,CASTLE,
CASTLE_INNER_BLOCK, CASTLE_TREE, BULLET,  CASTLE_CHECKPOINT};
enum ActionType {LEFT = 1, RIGHT, JUMP, SHOOT};
enum ButtonType { START_BUTTON, EXIT_BUTTON };
struct Button
{
    RECT rect;
    ButtonType type;
};

#endif // OBJECT_H
