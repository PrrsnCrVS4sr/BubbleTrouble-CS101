#include <simplecpp>
#include <ctime>
#include <chrono>
#include "shooter.h"
#include "bubble.h"

using namespace std::chrono;
/* Simulation Vars */
const double STEP_TIME = 0.01;

/* Game Vars */
int LEVEL_INDEX =1; //tells you which level you're on
int SCORE = 0; //current score of the player
const double FIRE_RATE= 4; //number of bullets fired in one second
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int RIGHT_MARGIN = WINDOW_X - 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);
const double TIME = 50; // Time per level


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

        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, -BUBBLE_DEFAULT_VX, 0, COLOR(216,191,216)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX, 0, COLOR(216,191,216)));

    }
    else if(level_index ==3)
    {

        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, -BUBBLE_DEFAULT_VX, 0, COLOR(75,0,130)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX, 0, COLOR(75,0,130)));


    }
    else if(level_index ==4)
    {
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, -BUBBLE_DEFAULT_VX, 0, COLOR(75,0,130)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX, 0, COLOR(75,0,130)));
        bubbles.push_back(Bubble(5*WINDOW_X/8.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX, 0, COLOR(216,191,216)));

    }
    else
    {
         bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, -BUBBLE_DEFAULT_VX, 0, COLOR(75,0,130)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX, 0, COLOR(75,0,130)));
        bubbles.push_back(Bubble(5*WINDOW_X/8.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX,BUBBLE_DEFAULT_VX , COLOR(75,0,130)));

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
            //get the distance between the bubble and bullet center
            double dist = distance(bubbles[i].get_center_x(),bubbles[i].get_center_y(),bullets[j].get_center_x(),bullets[j].get_center_y());
            //the minimum distance between the bullet and the bubble where collision doesnt happen.
            double collisionRadius =(bubbles[i].get_radius()+ (sqrt(pow(bullets[j].get_width()*0.5,2)+pow(bullets[j].get_height()*0.5,2))));

            if(dist<= collisionRadius)
            {



                vector<Bubble> newBubbles= bubbles[i].Split(); //splits the larger bubble into two

                if(newBubbles.size()!= 0) //As long as the larger bubble can be split, add the smaller bubbles into the Bubbles vector
                {
                    bubbles.push_back(newBubbles[0]);
                    bubbles.push_back(newBubbles[1]);


                }
                bullets.erase(bullets.begin()+j); //delete the bubble
                bubbles.erase(bubbles.begin()+i); //delete the bullet
                return 1;



            }

        }


    }
    return 0;


}


bool CollisionWithPlayer(vector<Bubble> &bubbles,Shooter player)
{
    for(unsigned int i =0; i < bubbles.size(); i++) // check for collision between each bubble and player
    {
        //distance between shooter head and bubble
        double distBWhead = distance(bubbles[i].get_center_x(),bubbles[i].get_center_y(),player.get_head_center_x(),player.get_head_center_y());
        //min distance for which collision does not happen between shooter head and bubble
        double collisionRadiusHead = (bubbles[i].get_radius()+ player.get_head_radius());


        //distance between shooter body and bubble
        double distBWbody = distance(bubbles[i].get_center_x(),bubbles[i].get_center_y(),player.get_body_center_x(),player.get_body_center_y());
        //min distance for which collision does not happen between shooter body and bubble
        double collisionAreaBody = bubbles[i].get_radius()+ player.get_body_width()*0.5;






        //send collision messages
        if(distBWbody<= collisionAreaBody || distBWhead<= collisionRadiusHead)
        {
            return true;
        }

    }
    return false;


}
string toString(int n)  // converts integer to a string
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

void StartLevel(int levelIndex,vector<Bubble>& bubbles) //starts each level
{

    Text LevelStartText(WINDOW_X/2,WINDOW_Y/2,"Level: " + toString(levelIndex));
    LevelStartText.setColor(COLOR("red"));
    wait(0.75);
    bubbles = create_bubbles(levelIndex);
}
void ResetScore() //resets score back to 0
{
    SCORE = 0;

}
void GameOver(string condition) // handles Game Over states
{
    if(condition == "lose")
    {
        Text loseText(WINDOW_X/2,WINDOW_Y/2,"GAME OVER!");
        loseText.setColor(COLOR("red"));
        string endScore = "Your Score: "+toString(SCORE);
        Text Score(WINDOW_X/2,2*WINDOW_Y/3,endScore);
        Score.setColor(COLOR("blue"));
        wait(2);
        ResetScore();
        LEVEL_INDEX = 1;

    }
    else if(condition == "win")
    {


        if(LEVEL_INDEX == 5) // executed when player beats all levels
        {
            Text winText(WINDOW_X/2,WINDOW_Y/2,"CONGRATULATIONS YOU WIN");
            winText.setColor(COLOR("green"));
            string endScore = "Your Score: "+toString(SCORE);
            Text Score(WINDOW_X/2,2*WINDOW_Y/3,endScore);
            Score.setColor(COLOR("blue"));
            wait(2);


        }
        else // executed when  player beats each level
        {
            Text winText(WINDOW_X/2,WINDOW_Y/2,"CONGRATULATIONS");
            winText.setColor(COLOR("green"));
            string endScore = "Your Score: "+toString(SCORE);
            Text Score(WINDOW_X/2,2*WINDOW_Y/3,endScore);
            Score.setColor(COLOR("blue"));
            wait(2);


        }
        LEVEL_INDEX++;

    }

}


int main()
{
    double WAIT_TIME = 0.75; // If there is a collision event between the player and the bubble, it is the time during which the player is invulnerable
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);





    while(LEVEL_INDEX<=5) // Game Loop as long as the Level is within the level index
    {


        Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
        b1.setColor(COLOR(0, 0, 255));

        // Initialize the bubbles
        vector<Bubble> bubbles;
        StartLevel(LEVEL_INDEX,bubbles);
        // Intialize the shooter
        Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);




        // Initialize the bullets (empty)
        vector<Bullet> bullets;

        string msg_cmd("Cmd: _");


        Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

        //GAME UI
        //Level Text
        string currentLevel = "Level "+toString(LEVEL_INDEX)+"/5";
        Text level((RIGHT_MARGIN+LEFT_MARGIN)/2,BOTTOM_MARGIN,currentLevel);

        //Health Text
        string health_msg;
        Text Health(RIGHT_MARGIN, TOP_MARGIN, health_msg);
        Health.setColor(COLOR("red"));

        //Time Tracker Text
        string cTime;
        Text Time((RIGHT_MARGIN+LEFT_MARGIN)/2, TOP_MARGIN, cTime);
        Time.setColor(COLOR("blue"));

        //Score Text
        string cScore;
        Text Score(LEFT_MARGIN,TOP_MARGIN,cScore);
        Score.setColor(COLOR("green"));






        XEvent event;

        int currentHP = shooter.get_health();
        int totalHP = shooter.get_health();

        bool has_collided_with_player = false;
        double track_time = TIME;

        //create a variable of type time_point from high_resolution_clock namespace
        high_resolution_clock::time_point lastFired;
        // Main game loop
        while (true) // Game Loop
        {
            //Get current time
            high_resolution_clock::time_point startTime = high_resolution_clock::now();

            high_resolution_clock::time_point endTime;




            // will be changed later
            //cout<<startTime<<endl;;



            //send updated health message
            health_msg = "Health: " +toString(currentHP) +"/"+ toString(totalHP);
            Health.setMessage(health_msg);

            //send updated time message
            cTime = "Time: " +toString(track_time);
            Time.setMessage(cTime);

            //send updated score message
            cScore = "Score: " +toString(SCORE);
            Score.setMessage(cScore);

            //calculate time between shots
            duration<double> time_diff_between_shots = duration_cast<duration<double>>(startTime - lastFired);

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
                else if(c == 'w' && time_diff_between_shots.count()> 1/FIRE_RATE) //shooter is limited by the firerate
                {
                    lastFired = high_resolution_clock::now();
                    bullets.push_back(shooter.shoot());
                }

                else if(c == 'q')
                    return 0;
            }

            // Update the bubbles
            move_bubbles(bubbles);

            // Update the bullets
            move_bullets(bullets);

            int scoreIncrement = CollisionOfBulletWithBubble(bubbles,bullets); //checks collision between bubble and bullet and gets the score increment
            has_collided_with_player = CollisionWithPlayer(bubbles,shooter); //checks collision between bubble and shooter
            SCORE += scoreIncrement;

            //Level is completed when all bubbles are destroyed
            if(bubbles.size()==0)
            {
                GameOver("win");
                track_time = TIME;
                break;


            }


            if(has_collided_with_player && WAIT_TIME>=0.75) // if the player is currently waiting, ie WAIT_TIME is less that 0.75 DO NOT EXECUTE
            {
                WAIT_TIME = 0;
                currentHP--;
                SCORE--;
                track_time -=4;


                shooter.ChangeColourForAMoment(0.2); //Change color when collision is detected
                if(currentHP<=0)
                {
                    GameOver("lose");
                    break;

                }
                endTime = high_resolution_clock::now();
                duration<double> time_span = duration_cast<duration<double>>(endTime - startTime);
                track_time -= (time_span).count();
                continue;




            }
            //If the timer goes below 0, the Game is Over
            if(track_time<=0)
            {
                GameOver("lose");
                break;

            }
            wait(STEP_TIME);
            endTime = high_resolution_clock::now();
            duration<double> time_span = duration_cast<duration<double>>(endTime - startTime);
            track_time -= (time_span).count();


            //After the collision between shooter and bubble occurs, the WAIT_TIME var is updated, every iteration of the while loop
            //The shooter gets about 0.75 seconds of invulnerability
            if(WAIT_TIME<0.75)
            {

                WAIT_TIME += (time_span).count();

            }

        }


    }
}

