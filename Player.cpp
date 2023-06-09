#include "Player.h"

Player::Player(SDL_Texture *p_tex, SDL_Texture *hp_tex)
{
    mTexture = p_tex;
    hpTexture = hp_tex;
    frame = 0;
    eFrame = 0;
    status = NONE;
    on_ground = false;
    direction = 1;
    HP = HP_MAX;
    mVelX = 0;
    mVelY = 0;
    animations.push_back(idle);
    animations.push_back(attack1);
    animations.push_back(attack2);
    animations.push_back(attack3);
    animations.push_back(dead);
    animations.push_back(defend);
    animations.push_back(hurt);
    animations.push_back(jump);
    animations.push_back(run);

    SDL_Rect hp1 = {1024, 1152, 32, 32};
    SDL_Rect hp2 = {2448, 528, 32, 32};
    SDL_Rect hp3 = {3424, 704, 32, 32};
    SDL_Rect hp4 = {4352, 864, 32, 32};
    SDL_Rect hp5 = {4848, 656, 32, 32};
    potions.push_back(hp1);
    potions.push_back(hp2);
    potions.push_back(hp3);
    potions.push_back(hp4);
    potions.push_back(hp5);

    setClips();

    mBox = {0, 0, 64, 80};
}

void Player::setClips()
{
    int n = 0;
    for(int i = 0; i < animations.size(); i++)
    {
        for(int j = 0; j < animations[i].amount_of_frame; j++)
        {
            m_playerClips[n].x = j*animations[i].frame_width;
            m_playerClips[n].y = (i+1)*160 - animations[i].frame_height;
            m_playerClips[n].h = animations[i].frame_height;
            m_playerClips[n].w = animations[i].frame_width;
            n++;
        }
    }
}

void Player::handleEvent( SDL_Event& event, GameState &state, int &hp)
{
    if(HP <= 0) return;
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_d:
            status = RIGHT;
            input_type.run = 1;
            input_type.right = 1;
            input_type.left = 0;
            input_type.defend = 0;
            break;
        case SDLK_a:
            status = LEFT;
            input_type.run = 1;
            input_type.left = 1;
            input_type.right = 0;
            input_type.defend = 0;
            break;
        case SDLK_w:
            if(input_type.jump == 0)
            {
                input_type.jump = 1;
                frame = 0;
            }
            break;
        case SDLK_s:
            input_type.defend = 1;
            input_type.run = 0;
            break;
        case SDLK_j:
            if(input_type.attack1 == 0)
            {
                input_type.attack1 = 1;
                input_type.run = 0;
                frame = 0;
            }
            break;
        case SDLK_k:
            if(input_type.attack2 == 0)
            {
                input_type.attack2 = 1;
                input_type.run = 0;
                frame = 0;
            }
            break;
        case SDLK_l:
            if(input_type.attack3 == 0)
            {
                input_type.attack3 = 1;
                input_type.run = 0;
                frame = 0;
            }
            break;
        case SDLK_e:
            if(hp > 0){
                HP += min(5, HP_MAX - HP);
                hp--;
            }
            break;
        case SDLK_ESCAPE:
            input_type.run = 0;
            input_type.left = 0;
            input_type.right = 0;
            state = STATE_PAUSE_MENU;
            break;
        }
    }
    else if(event.type == SDL_KEYUP)
    {
        switch(event.key.keysym.sym)
        {
        case SDLK_d:
            input_type.right = 0;
            input_type.run = 0;
            break;
        case SDLK_a:
            input_type.left = 0;
            input_type.run = 0;
            break;
        case SDLK_s:
            input_type.defend = 0;
            break;
        }
    }
}

void Player::show(RenderWindow &window, SDL_Rect &mCamera)
{
    int dxr = 0, dxl = 0, dy = 0, slow = 1, t = 0;
    currentBox = mBox;
    if(HP <= 0)
    {
        dxr = dead.denta_x_right, dxl = dead.denta_x_left, dy = dead.denta_y;
        slow = dead.slow_down;
        currentBox.w = dead.frame_width;
        currentBox.h = dead.frame_height;
        for(int i = 0; i <dead.index; i++)
        {
            t += animations[i].amount_of_frame;
        }
        frame++;
        if(frame >= dead.amount_of_frame*slow)
        {
            frame = 0;
            HP = 0;
            input_type.dead = 1;
        }
    }
    else if(input_type.defend == 1)
    {
        dxr = defend.denta_x_right, dxl = defend.denta_x_left, dy = defend.denta_y;
        slow = defend.slow_down;
        currentBox.w = defend.frame_width;
        currentBox.h = defend.frame_height;
        for(int i = 0; i <defend.index; i++)
        {
            t += animations[i].amount_of_frame;
        }
        frame = ++frame%(defend.amount_of_frame*slow);
    }
    else if(input_type.hurt == 1)
    {
        dxr = hurt.denta_x_right, dxl = hurt.denta_x_left, dy = hurt.denta_y;
        slow = hurt.slow_down;
        currentBox.w = hurt.frame_width;
        currentBox.h = hurt.frame_height;
        for(int i = 0; i <hurt.index; i++)
        {
            t += animations[i].amount_of_frame;
        }
        frame++;
        if(frame >= hurt.amount_of_frame*slow)
        {
            frame = 0;
            input_type.hurt = 0;
        }
    }
    else if(input_type.attack1 == 1)
    {
        dxr = attack1.denta_x_right, dxl = attack1.denta_x_left, dy = attack1.denta_y;
        slow = attack1.slow_down;
        currentBox.w = attack1.frame_width;
        currentBox.h = attack1.frame_height;
        for(int i = 0; i <attack1.index; i++)
        {
            t += animations[i].amount_of_frame;
        }
        frame++;
        if(frame >= attack1.amount_of_frame*slow)
        {
            frame = 0;
            input_type.attack1 = 0;
        }
    }
    else if(input_type.attack2 == 1)
    {
        dxr = attack2.denta_x_right, dxl = attack2.denta_x_left, dy = attack2.denta_y;
        slow = attack2.slow_down;
        currentBox.w = attack2.frame_width;
        currentBox.h = attack2.frame_height;
        for(int i = 0; i <attack2.index; i++)
        {
            t += animations[i].amount_of_frame;
        }
        frame++;
        if(frame >= attack2.amount_of_frame*slow)
        {
            frame = 0;
            input_type.attack2 = 0;
        }
    }
    else if(input_type.attack3 == 1)
    {
        dxr = attack3.denta_x_right, dxl = attack3.denta_x_left, dy = attack3.denta_y;
        slow = attack3.slow_down;
        currentBox.w = attack3.frame_width;
        currentBox.h = attack3.frame_height;
        for(int i = 0; i <attack3.index; i++)
        {
            t += animations[i].amount_of_frame;
        }
        frame++;
        if(frame >= attack3.amount_of_frame*slow)
        {
            frame = 0;
            input_type.attack3 = 0;
        }
    }
    else if(!on_ground)
    {
        dxr = jump.denta_x_right, dxl = jump.denta_x_left, dy = jump.denta_y;
        slow = jump.slow_down;
        currentBox.w = jump.frame_width;
        currentBox.h = jump.frame_height;
        for(int i = 0; i <jump.index; i++)
        {
            t += animations[i].amount_of_frame;
        }
        frame++;
        if(frame >= jump.amount_of_frame*slow)
        {
            frame = 0;
            input_type.jump = 0;
        }
    }
    else if(input_type.run == 1)
    {
        dxr = run.denta_x_right, dxl = run.denta_x_left, dy = run.denta_y;
        slow = run.slow_down;
        currentBox.w = run.frame_width;
        currentBox.h = run.frame_height;
        for(int i = 0; i <run.index; i++)
        {
            t += animations[i].amount_of_frame;
        }
        frame = ++frame%(run.amount_of_frame*slow);
    }
    else
    {
        dxr = idle.denta_x_right, dxl = idle.denta_x_left, dy = idle.denta_y;
        slow = idle.slow_down;
        currentBox.w = idle.frame_width;
        currentBox.h = idle.frame_height;
        for(int i = 0; i <idle.index; i++)
        {
            t += animations[i].amount_of_frame;
        }
        frame++;
        if(frame >= idle.amount_of_frame*slow)
        {
            frame = 0;
            input_type.idle = 0;
        }
    }
    SDL_Rect* current_clip = &m_playerClips[t + frame/slow];
    if(status == RIGHT || status == NONE)
    {
        window.render(mTexture, currentBox.x - mCamera.x - dxr, currentBox.y - mCamera.y - dy, currentBox.w, currentBox.h, current_clip);
    }
    else
    {
        window.render(mTexture, currentBox.x - mCamera.x - dxl, currentBox.y - mCamera.y - dy, currentBox.w, currentBox.h, current_clip, 0.0, NULL, SDL_FLIP_HORIZONTAL);
    }
    SDL_Rect b = {currentBox.x - mCamera.x - dxr, currentBox.y - mCamera.y - dy, currentBox.w, currentBox.h};
    for(int i = 0; i < potions.size(); i++){
        window.render(hpTexture, potions[i].x - mCamera.x, potions[i].y - mCamera.y, potions[i].w, potions[i].h);
    }
    window.render(hpTexture, 50, 50, 32, 32);
}

void Player::doPlayer(Tile *map_data)
{
    mVelX = 0;
    mVelY += GRAVITY_SPEED;
    if(mVelY >= MAX_FALL_SPEED)
    {
        mVelY = MAX_FALL_SPEED;
    }
    if(input_type.hurt == 0)
    {
        if(input_type.run == 1)
        {
            if(input_type.left == 1)
            {
                mVelX -= PLAYER_SPEED;
            }
            else if(input_type.right == 1)
            {
                mVelX += PLAYER_SPEED;
            }
        }
        if(input_type.jump == 1)
        {
            if(on_ground == true)
            {
                if(mVelY > 0) mVelY -= PLAYR_JUMP_VAL;
                on_ground = false;
            }
            input_type.jump = 0;
        }
    }
    if(HP <= 0)
    {
        mVelX = 0;
    }
    checkToMap(map_data);
}

void Player::checkToMap(Tile *map_data)
{
    mBox.x += mVelX;
    if( ( mBox.x < 0 ) || ( mBox.x + mBox.w > LEVEL_WIDTH ) || touchesWall( map_data ) )
    {
        mBox.x -= mVelX;
    }
    mBox.y += mVelY;
    if( ( mBox.y < 0 ) || touchesWall( map_data)  )
    {
        on_ground = true;
        mBox.y -= mVelY;
    }
    else on_ground = false;
}

void Player::centerPlayerOnMap(SDL_Rect &mCamera)
{
    mCamera.x = ( mBox.x + mBox.w / 2 ) - SCREEN_WIDTH / 2;
    mCamera.y = ( mBox.y + mBox.h / 2 ) - SCREEN_HEIGHT / 2;

    if( mCamera.x < 0 )
    {
        mCamera.x = 0;
    }
    if( mCamera.y < 0 )
    {
        mCamera.y = 0;
    }
    if( mCamera.x > LEVEL_WIDTH - mCamera.w )
    {
        mCamera.x = LEVEL_WIDTH - mCamera.w;
    }
    if( mCamera.y > LEVEL_HEIGHT - mCamera.h )
    {
        mCamera.y = LEVEL_HEIGHT - mCamera.h;
    }
}

bool Player::checkCollision( SDL_Rect a, SDL_Rect b)
{
    int leftA = a.x, leftB = b.x;
    int rightA = a.x + a.w, rightB = b.x + b.w;
    int topA = a.y, topB = b.y;
    int bottomA = a.y + a.h, bottomB = b.y + b.h;

    if( bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
    {
        return false;
    }
    return true;
}

bool Player::touchesWall(Tile *map_data)
{
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        if(map_data[i].getType() != -1 && map_data[i].getType() != ENEMY_TILE_TYPE)
        {
            if( checkCollision( mBox, map_data[i].getBox()) )
            {
                return true;
            }
        }
    }
    return false;
}

void Player::drawHP(SDL_Renderer &ren, SDL_Rect &mCamera)
{
    SDL_Rect health_box = {mBox.x + mBox.w/4 - HP_MAX*3/5 - mCamera.x, mBox.y - 10 - mCamera.y, HP*3, 6};

    if(HP > 0)
    {
        SDL_SetRenderDrawColor(&ren, 255, 0, 0, 255);
        SDL_RenderFillRect(&ren, &health_box);
    }
    SDL_SetRenderDrawColor(&ren, 0, 250, 154, 255);
    for(int i = 0; i < 5; i++)
    {
        health_box = {mBox.x + mBox.w/4 - HP_MAX*3/5 - mCamera.x + i*HP_MAX*3/5, mBox.y - 10 - mCamera.y, HP_MAX*3/5, 6};
        if(HP <= 0)
        {
            if(status == LEFT) health_box.x = mBox.x + mBox.w/2 - HP_MAX/2 - mCamera.x + i*HP_MAX*3/5;
            else health_box.x = mBox.x + mBox.w/2 - mCamera.x + i*HP_MAX*3/5;
        }
        SDL_RenderDrawRect(&ren, &health_box);
    }
}

void Player::isHittingPotion(int &hp){
    for(int i = 0; i < potions.size(); i++){
        if(checkCollision(potions[i], mBox)){
            if(input_type.attack1 == 1 || input_type.attack2 == 1 || input_type.attack3 == 1){
                hp++;
                potions.erase(potions.begin() + i);
            }
        }
    }
}

void Player::isHitted(bool hitted, int enemy_status)
{
    if(hitted)
    {
        HP--;
        if(input_type.hurt == 0)
        {;
            input_type.hurt = 1;
            frame = 0;
        }
    }
}

bool Player::isDead()
{
    if(input_type.dead == 1 || ( mBox.y + mBox.h > LEVEL_HEIGHT )) return true;
    else return false;
}
