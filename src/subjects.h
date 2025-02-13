typedef struct Item {
    float x;
    float y;
    float width;
    float height;
    float y_speed;
} Item;

extern Item item_power[100];

typedef struct Player {
    float x;
    float y;
    float width;
    float height;
    float x_speed;
    float y_speed;
    float frame_index;
    int hit_points;
} Player;

extern Player player;

typedef struct Bullet {
    float x;
    float y;
    float width;
    float height;
    float x_speed;
    float y_speed;
    float k; //angle coefficient
    float b; //Oy
} Bullet;

extern Bullet player_bullet[1000];
extern Bullet power_bullet[1000];
extern Bullet pixi_bullet[100];

typedef struct Objects {
    float x;
    float y;
    float y_speed;
    float x_speed;
    int number;
    float hit_points;
    float height;
    float width;
} Objects;

extern Objects pixi[25];
extern Objects player_power[4];

extern double power;
extern int pow_count;
extern int opacity;
extern int enemies_count;
extern int power_bullet_count;