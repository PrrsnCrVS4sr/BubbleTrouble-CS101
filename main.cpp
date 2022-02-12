#include <simplecpp>
#include <ctime>
#include "shooter.h"
#include "bubble.h"


/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
int LEVEL_INDEX =1; //tells you which level you're on
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int RIGHT_MARGIN = WINDOW_X - 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);
const double TIME = 45; // Time per level


void move_bullets(vector<Bullet> &bullets)
{
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++)
    {
        if(!bullets[i].nextStep(STEP_TIME))
        {
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles)
{
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles(int level_index)
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    // different start conditions for different levels
    if(level_index == 1)
    {
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));


    }
    else if(level_index == 2)
    {

        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));

    }
    else
    {

        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));


    }

    return bubbles;
}

double distance(double x1,double y1, double x2, double y2) // calculates the distance between two points
{
    double d = sqrt(pow(x2-x1,2)+pow(y2-y1,2));
    return d;


}

int CollisionOfBulletWithBubble(vector<Bubble> &bubbles,vector<Bullet> &bullets)
{
    for(unsigned int i = 0; i< bubbles.size(); i++)
    {
        for(unsigned int j =0 ; j<bullets.size(); j++)
        {
            double dist = distance(bubbles[i].get_center_x(),bubbles[i].get_center_y(),bullets[j].get_center_x(),bullets[j].get_center_y());
            double collisionRadius =(bubbles[i].get_radius()+ (sqrt(pow(bullets[j].get_width()*0.5,2)+pow(bullets[j].get_height()*0.5,2))));

            if(dist<= collisionRadius)
            {
                vector<Bubble> newBubbles= bubbles[i].Split();

                if(newBubbles.size()!= 0)
                {
                    bubbles.push_back(newBubbles[0]);
                    bubbles.push_back(newBubbles[1]);


                }
                bullets.erase(bullets.begin()+j);
                bubbles.erase(bubbles.begin()+i);
                return 1;

            }

        }


    }
    return 0;


}


bool CollisionWithPlayer(vector<Bubble> &bubbles,Shooter player)
{
    for(unsigned int i =0; i < bubbles.size(); i++)
    {
        double distBWhead = distance(bubbles[i].get_center_x(),bubbles[i].get_center_y(),player.get_head_center_x(),player.get_head_center_y());
        double distBWbody = distance(bubbles[i].get_center_x(),bubbles[i].get_center_y(),player.get_body_center_x(),player.get_body_center_y());
        double collisionAreaBody = (bubbles[i].get_radius()+ (sqrt(pow(player.get_body_width()*0.5,2)+pow(player.get_body_height()*0.5,2))));
        double collisionRadiusHead = (bubbles[i].get_radius()+ player.get_head_radius());
        if(distBWbody<= collisionAreaBody || distBWhead<= collisionRadiusHead)
        {
            return true;
        }

    }
    return false;


}
string to_string(int n)  // converts integer to a string
{

    if(n==0)
    {
        return "0";
    }
    string s = "";
    while(n>0)
    {
        int digit = n%10;
        char dig = digit +'0';
        n /=10;
        s += dig;

    }
    string rev_str = "";
    for(int i = s.size()-1; i>=0; i--)
    {
        rev_str=rev_str+ s[i];

    }
    return rev_str;

}

void StartLevel(int levelIndex,vector<Bubble>* bubbles)
{

    Text LevelStartText(WINDOW_X/2,WINDOW_Y/2,"Level: " + to_string(levelIndex));
    LevelStartText.setColor(COLOR("red"));
    wait(0.75);
    *bubbles = create_bubbles(levelIndex);
}

void GameOver(string condition)
{
    if(condition == "lose")
    {
        Text loseText(WINDOW_X/2,WINDOW_Y/2,"GAME OVER!");
        loseText.setColor(COLOR("red"));
        wait(2);

    }
    else if(condition == "win")
    {


        if(LEVEL_INDEX == 3)
        {
            Text winText(WINDOW_X/2,WINDOW_Y/2,"CONGRATULATIONS YOU BEAT ALL LEVELS");
            winText.setColor(COLOR("green"));
            wait(2);


        }
        else // Endsieg(Final victory) condition
        {
            Text winText(WINDOW_X/2,WINDOW_Y/2,"CONGRATULATIONS");
            winText.setColor(COLOR("green"));
            wait(2);


        }
        LEVEL_INDEX++;

    }

}
int main()
{
    double WAIT_TIME = 0.75; // If there is a collision event between the player and the bubble, it is the time during which the player is invulnerable
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    int score;



    while(LEVEL_INDEX<=3) // Game Loop as long as the Level is within the level index
    {
        score = (LEVEL_INDEX-1)*pow(2,LEVEL_INDEX-1);

        Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
        b1.setColor(COLOR(0, 0, 255));

        // Initialize the bubbles
        vector<Bubble> bubbles;
        StartLevel(LEVEL_INDEX,&bubbles);
        // Intialize the shooter
        Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);




        // Initialize the bullets (empty)
        vector<Bullet> bullets;

        string msg_cmd("Cmd: _");


        Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

        string currentLevel = "Level "+to_string(LEVEL_INDEX)+"/3";
        Text level((RIGHT_MARGIN+LEFT_MARGIN)/2,BOTTOM_MARGIN,currentLevel);

        string health_msg;
        Text Health(RIGHT_MARGIN, TOP_MARGIN, health_msg);
        Health.setColor(COLOR("red"));

        string cTime;
        Text Time((RIGHT_MARGIN+LEFT_MARGIN)/2, TOP_MARGIN, cTime);
        Time.setColor(COLOR("blue"));

        string cScore;
        Text Score(LEFT_MARGIN,TOP_MARGIN,cScore);
        Score.setColor(COLOR("green"));






        XEvent event;

        int currentHP = shooter.get_health();
        int totalHP = shooter.get_health();

        bool has_collided_with_player = false;
        double track_time = TIME;
        // Main game loop
        while (true) // Loop that deals with gameplay
        {
            time_t startTime,endTime;

            startTime = time(NULL);

            // will be changed later
            //cout<<startTime<<endl;;



            //Health UI
            health_msg = "Health: " +to_string(currentHP) +"/"+ to_string(totalHP);
            Health.setMessage(health_msg);

            //Time UI
            cTime = "Time: " +to_string(track_time);
            Time.setMessage(cTime);


            cScore = "Score: " +to_string(score);
            Score.setMessage(cScore);



            bool pendingEvent = checkEvent(event);
            if (pendingEvent)
            {
                // Get the key pressed
                char c = charFromEvent(event);
                msg_cmd[msg_cmd.length() - 1] = c;
                charPressed.setMessage(msg_cmd);

                // Update the shooter
                if(c == 'a')
                    shooter.move(STEP_TIME, true);
                else if(c == 'd')
                    shooter.move(STEP_TIME, false);
                else if(c == 'w')
                    bullets.push_back(shooter.shoot());
                else if(c == 'q')
                    return 0;
            }

            // Update the bubbles
            move_bubbles(bubbles);

            // Update the bullets
            move_bullets(bullets);

            int scoreIncrement = CollisionOfBulletWithBubble(bubbles,bullets);
            has_collided_with_player = CollisionWithPlayer(bubbles,shooter);
            score += scoreIncrement;
            if(bubbles.size()==0)
            {
                GameOver("win");
                track_time = TIME;
                break;


            }
            if(has_collided_with_player && WAIT_TIME>=0.75) // if the player is currently waiting, ie WAIT_TIME is less that 0.3 DO NOT EXECUTE
            {
                WAIT_TIME = 0;
                currentHP--;

                shooter.ChangeColourForAMoment(0.2);
                if(currentHP<=0)
                {
                    GameOver("lose");
                    break;

                }
                endTime = time(NULL);
                track_time -= difftime(endTime,startTime);
                continue;




            }

            if(track_time<=0)
            {
                GameOver("lose");
                break;

            }
            wait(STEP_TIME);
            endTime = time(NULL);

            track_time -= difftime(endTime,startTime);
            cout << difftime(endTime, startTime)<<endl;
            if(WAIT_TIME<0.75)
            {

                WAIT_TIME += difftime(endTime,startTime);

            }

        }


    }
}

