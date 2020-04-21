#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/**
 * Grab Snaffles and try to throw them through the opponent's goal!
 * Move towards a Snaffle to grab it and use your team id to determine towards where you need to throw it.
 * Use the Wingardium spell to move things around at your leisure, the more magic you put it, the further they'll move.
 **/

int main()
{
    vector<int> Wx(15);
    vector<int> Wy(15);
    vector<int> Sx(20);
    vector<int> Sy(20);
    vector<int> Sid(20);
    typedef struct snaffle{
        int myTeamId;
        int myScore;
        int myMagic;
        int opponentScore;
        int opponentMagic;
        int entities;
        int Id; // entity identifier
        string entityType; // "WIZARD", "OPPONENT_WIZARD" or "SNAFFLE" or "BLUDGER"
        int x; // position
        int y; // position
        int vx; // velocity
        int vy; // velocity
        int state; // 1
    } t_info;
    t_info S[20];
    t_info W[20];
    t_info OW[20];
    int goalx, goaly;
    int mygoalx, mygoaly;
    int myTeamId; // if 0 you need to score on the right of the map, if 1 you need to score on the left
    cin >> myTeamId; cin.ignore();
    if (myTeamId == 0)
    {
        goalx = 16000;
        goaly = 3750;
        mygoalx = 0;
        mygoaly = 3750;
    }
    else {
        goalx = 0;
        goaly = 3750;
        mygoalx = 16000;
        mygoaly = 3750;
    }
    int myMagic;
    int j = 0;
    int o = 0;
    int p = 0;
    int snaffle = 0;
    int myScore;
    int opponentScore;
    int check = 0;

    // game loop
    while (1) {
        //int myScore;
        //int myMagic;
        cin >> myScore >> myMagic; cin.ignore();
        //int opponentScore;
        int opponentMagic;
        cin >> opponentScore >> opponentMagic; cin.ignore();
        int entities; // number of entities still in game
        cin >> entities; cin.ignore();
        //cout << entities << endl;
        for (int i = 0; i < entities; i++) {
            int entityId; // entity identifier
            string entityType; // "WIZARD", "OPPONENT_WIZARD" or "SNAFFLE" or "BLUDGER"
            int x; // position
            int y; // position
            int vx; // velocity
            int vy; // velocity
            int state; // 1 if the wizard is holding a Snaffle, 0 otherwise. 1 if the Snaffle is being held, 0 otherwise. id of the last victim of the bludger.
            cin >> entityId >> entityType >> x >> y >> vx >> vy >> state; cin.ignore();
            snaffle = entities - 6;
            if (entityType == "SNAFFLE")
            {
                Sx[i] = x;
                Sy[i] = y;
                Sid[i] = entityId;
            }
            // if (entityType  == "SNAFFLE" && snaffle % 2 == 0)
            // {
            //     if (j % 2 == 0)
            //         j = 0;
            //     Sx[i] = x;
            //     Sy[i] = y;
            //     Sid[i] = entityId;
            //     // S[j].x = x;
            //     // S[j].y = y;
            //     S[j].Id = entityId;
            //     j++;
            // }
            // else if (entityType  == "SNAFFLE" && snaffle % 2 == 1)
            // {
            //     //cerr << i << endl;
            //     if (j == snaffle)
            //         j = 0;
            //     Sx[i] = x;
            //     Sy[i] = y;
            //     Sid[i] = entityId;
            //     // S[j].x = x;
            //     // S[j].y = y;
            //     S[j].Id = entityId;
            //     j++;
            // }
            else if (entityType == "WIZARD")
            {
                //cerr << i << endl;
                Wx[i] = x;
                Wy[i] = y;
                // W[i].x = x;
                // W[i].y = y;
                W[i].state = state;
                W[i].Id = entityId;
                W[i].myMagic = myMagic;
                if (entityId == 2 || entityId == 0)
                    j = entityId;
                else
                    o = entityId;
                //cerr << Wx[i] << endl;
            }
            else if (entityType == "OPPONENT_WIZARD")
            {
                if (i == 2 || i == 3)
                {
                    if (i == 2)
                        p = 0;
                    else if (i == 3)
                        p = 1;
                    OW[p].x = x;
                    OW[p].y = y;
                }
                else
                {
                    OW[i].x = x;
                    OW[i].y = y;
                }
            }

        }
        for (int i = 0; i < 2; i++) {
            int Scx[20];
            int Scy[20];
            int Sfx[20];
            int Sfy[20];
            int Scid[20];
            int Sfid[20];
            int Scgoal[20];
            int Scmygoal[20];
            int nearestgoal[3];
            int nearmygoal[3];
            int centernear[4];
            double closestSnaffle = 1e9;
            double farthestSnaffle = 0;
            nearestgoal[0] = 1e5;
            nearmygoal[0] = 1e5;
            centernear[3] = 1e5;
            for (int k = 4; k < 4 + snaffle; k++)
            {
                double tmp = sqrt((Wx[i] - Sx[k]) * (Wx[i] - Sx[k]) + (Wy[i] - Sy[k]) * (Wy[i] - Sy[k]));
                if (tmp < closestSnaffle)
                {
                    Scid[i] = Sid[k];
                    Scx[i] = Sx[k];
                    Scy[i] = Sy[k];
                }
                closestSnaffle = min(closestSnaffle, tmp);

                double tmp2 = sqrt((Wx[i] - Sx[k]) * (Wx[i] - Sx[k]) + (Wy[i] - Sy[k]) * (Wy[i] - Sy[k]));
                if (tmp2 > farthestSnaffle)
                {
                    Sfid[i] = Sid[k];
                    Sfx[i] = Sx[k];
                    Sfy[i] = Sy[k];
                }
                farthestSnaffle = max(farthestSnaffle, tmp2);

                int tmp3 = abs(Sx[k] - goalx);
                if (tmp3 < nearestgoal[0])
                {
                    Scgoal[0] = Sx[k];
                    Scgoal[1] = Sy[k];
                    Scgoal[2] = Sid[k];
                }
                nearestgoal[0] = min(tmp3, nearestgoal[0]);

                int tmp4 = abs(Sx[k] - mygoalx);
                if (tmp4 < nearmygoal[0])
                {
                    Scmygoal[0] = Sx[k];
                    Scmygoal[1] = Sy[k];
                    Scmygoal[2] = Sid[k];
                }
                nearmygoal[0] = min(tmp4, nearmygoal[0]);

                int tmp5 = sqrt((8000 - Sx[k]) * (8000 - Sx[k]) + (3750 - Sy[k]) * (3750 - Sy[k]));
                if (tmp5 < centernear[3])
                {
                    centernear[0] = Sx[k];
                    centernear[1] = Sy[k];
                    centernear[2] = Sid[k];
                }
                centernear[3] = min(tmp5, centernear[3]);
            }
            // cerr << Scid[i] << endl;
            // cerr << "-------------" << endl;

            int Scsecondx[20];
            int Scsecondy[20];
            int Scsecondgoal[20];
            int nearestgoal2[3];
            int Scsecondid[20];
            int centernear2[4];
            nearestgoal2[0] = 1e5;
            double SecondSnaffle = 1e9;
            centernear2[3] = 1e5;
            for (int k = 4; k < 4 + snaffle; k++)
            {
                double tmp = sqrt((Wx[i] - Sx[k]) * (Wx[i] - Sx[k]) + (Wy[i] - Sy[k]) * (Wy[i] - Sy[k]));
                if (Scx[i] != Sx[k] && Scy[i] != Sy[k])
                {
                    if (tmp < SecondSnaffle)
                    {
                        Scsecondx[i] = Sx[k];
                        Scsecondy[i] = Sy[k];
                        Scsecondid[i] = Sid[k];
                    }
                    SecondSnaffle = min(SecondSnaffle, tmp);
                }
                if (Scgoal[0] != Sx[k] && Scgoal[1] != Sy[k])
                {
                    int tmp3 = abs(Sx[k] - goalx);
                    if (tmp3 < nearestgoal2[0])
                    {
                        Scsecondgoal[0] = Sx[k];
                        Scsecondgoal[1] = Sy[k];
                        Scsecondgoal[2] = Sid[k];
                    }
                    nearestgoal2[0] = min(tmp3, nearestgoal2[0]);
                }

                if (centernear[0] != Sx[k] && centernear[1] != Sy[k])
                {
                    int tmp5 = sqrt((8000 - Sx[k]) * (8000 - Sx[k]) + (3750 - Sy[k]) * (3750 - Sy[k]));
                    if (tmp5 < centernear2[3])
                    {
                        centernear2[0] = Sx[k];
                        centernear2[1] = Sy[k];
                        centernear2[2] = Sid[k];
                    }
                    centernear2[3] = min(tmp5, centernear2[3]);
                }
            }
            // cerr << Scsecondid[i] << endl;
            // cerr << "-------------" << endl;


                int Scthirdx[20];
                int Scthirdy[20];
                int Scthirdid[20];
                double thirdSnaffle = 1e9;
                for (int k = 4; k < 4 + snaffle; k++)
                {
                    double tmp = sqrt((Wx[i] - Sx[k]) * (Wx[i] - Sx[k]) + (Wy[i] - Sy[k]) * (Wy[i] - Sy[k]));
                    if (Scx[i] != Sx[k] && Scy[i] != Sy[k] && Scsecondx[i] != Sx[k] && Scsecondy[i] != Sy[k])
                    {
                        if (tmp < thirdSnaffle)
                        {
                            Scthirdx[i] = Sx[k];
                            Scthirdy[i] = Sy[k];
                            Scthirdid[i] = Sid[k];
                        }
                        thirdSnaffle = min(thirdSnaffle, tmp);
                    }
                }

            // cerr << Scthirdid[i] << endl;
            // cerr << "-------------" << endl;


            int magi = myMagic/2;
            if ((myScore+snaffle+opponentScore-1) / 2 <= myScore)
            {
                if (myScore > opponentScore)
                {
                    if (W[i].state == 1)
                    {
                        cout << "THROW " << goalx << ' ' << goaly << " 500" << endl;
                    }
                    else if(W[i].state == 0)
                    {
                        double tmp1 = sqrt((Wx[0] - Scgoal[0]) * (Wx[0] - Scgoal[0]) + (Wy[0] - Scgoal[1]) * (Wy[0] - Scgoal[1]));
                        double tmp2 = sqrt((Wx[1] - Scgoal[0]) * (Wx[1] - Scgoal[0]) + (Wy[1] - Scgoal[1]) * (Wy[1] - Scgoal[1]));
                        if (W[i].myMagic >= 27)
                        {
                            if (tmp1 < tmp2)
                            {
                                if (W[i].Id == j)
                                {
                                    cout << "MOVE " << Scgoal[0] << ' ' <<  Scgoal[1] << " 150" << endl;
                                }
                                else
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;

                            }
                            else if (tmp2 <= tmp1)
                            {
                                if (W[i].Id == o)
                                {
                                    cout << "MOVE " << Scgoal[0] << ' ' <<  Scgoal[1] << " 150" << endl;
                                }
                                else
                                {
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                }
                            }
                        }
                        else
                        {
                            cout << "MOVE " << Scgoal[0] << ' ' <<  Scgoal[1] << " 150" << endl;
                        }
                    }
                }
                else
                {
                    if (abs(Scgoal[0] - goalx) <= 8000)
                    {
                        if (W[i].state == 1)
                        {
                            if (Scgoal[1] >= 2000 && Scgoal[1] <= 5500)
                                cout << "THROW " << goalx << ' ' << Scgoal[1] << " 500" << endl;
                            else
                                cout << "THROW " << goalx << ' ' << goaly << " 500" << endl;
                        }
                        else if(W[i].state == 0)
                        {
                            double tmp1 = sqrt((Wx[0] - Scmygoal[0]) * (Wx[0] - Scmygoal[0]) + (Wy[0] - Scmygoal[1]) * (Wy[0] - Scmygoal[1]));
                            double tmp2 = sqrt((Wx[1] - Scmygoal[0]) * (Wx[1] - Scmygoal[0]) + (Wy[1] - Scmygoal[1]) * (Wy[1] - Scmygoal[1]));
                            if (W[i].myMagic >= 15 && abs(Scmygoal[0] - goalx) < 2000)
                            {
                                if (tmp1 < tmp2)
                                {
                                    if (W[i].Id == j)
                                    {
                                        cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                    }
                                    else
                                    {
                                        if (Scgoal[1] >= 2000 && Scgoal[1] <= 5500)
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << Scmygoal[1] << ' ' << myMagic << endl;
                                        else
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    }

                                }
                                else if (tmp2 <= tmp1)
                                {
                                    if (W[i].Id == o)
                                    {
                                        cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                    }
                                    else
                                    {
                                        if (Scgoal[1] >= 2000 && Scgoal[1] <= 5500)
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << Scmygoal[1] << ' ' << myMagic << endl;
                                        else
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    }
                                }
                            }
                            else if (W[i].myMagic >= 30)
                            {
                                if (tmp1 < tmp2)
                                {
                                    if (W[i].Id == j)
                                    {
                                        cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                    }
                                    else
                                    {
                                        if (Scgoal[1] >= 2000 && Scgoal[1] <= 5500)
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << Scmygoal[1] << ' ' << myMagic << endl;
                                        else
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    }

                                }
                                else if (tmp2 <= tmp1)
                                {
                                    if (W[i].Id == o)
                                    {
                                        cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                    }
                                    else
                                    {
                                        if (Scgoal[1] >= 2000 && Scgoal[1] <= 5500)
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << Scmygoal[1] << ' ' << myMagic << endl;
                                        else
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    }
                                }
                            }
                            else
                            {
                                cout << "MOVE " << Scgoal[0] << ' ' <<  Scgoal[1] << " 150" << endl;
                            }
                        }
                    }
                    else
                    {
                        if (W[i].state == 1)
                        {
                            cout << "THROW " << goalx << ' ' << goaly << " 500" << endl;
                        }
                        else if(W[i].state == 0)
                        {
                            double tmp1 = sqrt((Wx[0] - Scmygoal[0]) * (Wx[0] - Scmygoal[0]) + (Wy[0] - Scmygoal[1]) * (Wy[0] - Scmygoal[1]));
                            double tmp2 = sqrt((Wx[1] - Scmygoal[0]) * (Wx[1] - Scmygoal[0]) + (Wy[1] - Scmygoal[1]) * (Wy[1] - Scmygoal[1]));
                            if (W[i].myMagic >= 20)
                            {
                                if (tmp1 < tmp2)
                                {
                                    if (W[i].Id == j)
                                    {
                                        cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                    }
                                    else
                                    {
                                        if (Scgoal[1] >= 3750)
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                        else
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << myMagic << endl;
                                    }

                                }
                                else if (tmp2 <= tmp1)
                                {
                                    if (W[i].Id == o)
                                    {
                                        cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                    }
                                    else
                                    {
                                        if (Scgoal[1] >= 3750)
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                        else
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << myMagic << endl;
                                    }
                                }
                            }
                            else
                            {
                                cout << "MOVE " << Scgoal[0] << ' ' <<  Scgoal[1] << " 150" << endl;
                            }
                        }
                    }
                }
            }
            else if ((myScore+snaffle+opponentScore-1) / 2 <= opponentScore && myScore != opponentScore)
            {
                if (snaffle >= opponentScore)
                {
                    if (W[i].state == 1)
                    {
                        cout << "THROW " << goalx << ' ' << goaly << " 500" << endl;
                    }
                    else if(W[i].state == 0)
                    {
                        double tmp1 = sqrt((Wx[0] - Scmygoal[0]) * (Wx[0] - Scmygoal[0]) + (Wy[0] - Scmygoal[1]) * (Wy[0] - Scmygoal[1]));
                        double tmp2 = sqrt((Wx[1] - Scmygoal[0]) * (Wx[1] - Scmygoal[0]) + (Wy[1] - Scmygoal[1]) * (Wy[1] - Scmygoal[1]));
                        if (W[i].myMagic >= 25 && abs(Scmygoal[0] - mygoalx) < 5000)
                        {
                            if (tmp1 < tmp2)
                            {
                                if (W[i].Id == j)
                                {
                                    cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                }
                                else
                                {
                                    if (Scmygoal[1] >= 2000 && Scmygoal[1] <= 5000)
                                    {
                                        if (Scmygoal[1] <= 3750)
                                        {
                                            if (myMagic >= 30)
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << 30 << endl;
                                            else
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << myMagic << endl;
                                        }
                                        else
                                        {
                                            if (myMagic >= 30)
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << 30 << endl;
                                            else
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                        }
                                    }
                                    else
                                    {
                                        cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    }
                                }

                            }
                            else if (tmp2 <= tmp1)
                            {
                                if (W[i].Id == o)
                                {
                                    cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                }
                                else
                                {
                                    if (Scmygoal[1] >= 2000 && Scmygoal[1] <= 5000)
                                    {
                                        if (Scmygoal[1] <= 3750)
                                        {
                                            if (myMagic >= 30)
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << 30 << endl;
                                            else
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << myMagic << endl;
                                        }
                                        else
                                        {
                                            if (myMagic >= 30)
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << 30 << endl;
                                            else
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                        }
                                    }
                                    else
                                    {
                                        cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    }
                                }
                            }
                        }
                        else if (myMagic >= 15 && abs(Scgoal[0] - goalx) < 1000 && Scid[i] != Scgoal[2])
                        {
                            double tmp1 = sqrt((Wx[0] - Scx[0]) * (Wx[0] - Scx[0]) + (Wy[0] - Scy[0]) * (Wy[0] - Scy[0]));
                            double tmp2 = sqrt((Wx[1] - Scx[1]) * (Wx[1] - Scx[1]) + (Wy[1] - Scy[1]) * (Wy[1] - Scy[1]));
                            if (tmp1 < tmp2)
                            {
                                if (W[i].Id == j)
                                {
                                    cout << "MOVE " << Scx[0] << ' ' <<  Scy[0] << " 150" << endl;
                                //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << magi << endl;
                                }
                                else
                                {
                                    if (Scgoal[1] >= 2000 && Scgoal[1] <= 5500)
                                    {
                                        if (Scid[0] != Scgoal[2])
                                            cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 10 << endl;
                                        else
                                        {
                                            if (Scid[0] == Scid[1])
                                                cout << "MOVE " << Scsecondx[1] << ' ' <<  Scsecondy[1] << " 150" << endl;
                                            else
                                                cout << "MOVE " << Scx[1] << ' ' <<  Scy[1] << " 150" << endl;
                                        }
                                    }
                                    else
                                    {
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 10 << endl;
                                    }
                                }
                            }
                            else if (tmp2 <= tmp1)
                            {
                                if (W[i].Id == o)
                                {
                                    cout << "MOVE " << Scx[1] << ' ' <<  Scy[1] << " 150" << endl;
                                //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                }
                                else
                                {
                                    if (Scgoal[1] > 2000 && Scgoal[1] < 5500)
                                    {
                                        if (Scid[1] != Scgoal[2])
                                            cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 10 << endl;
                                        else
                                        {
                                            if (Scid[0] == Scid[1])
                                                cout << "MOVE " << Scsecondx[0] << ' ' <<  Scsecondy[0] << " 150" << endl;
                                            else
                                                cout << "MOVE " << Scx[0] << ' ' <<  Scy[0] << " 150" << endl;
                                        }
                                    }
                                    else
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 10 << endl;
                                }
                            }
                        }
                        else
                        {
                            double tmp3 = sqrt((Wx[0] - Scmygoal[0]) * (Wx[0] - Scmygoal[0]) + (Wy[0] - Scmygoal[1]) * (Wy[0] - Scmygoal[1]));
                            double tmp4 = sqrt((Wx[1] - Scmygoal[0]) * (Wx[1] - Scmygoal[0]) + (Wy[1] - Scmygoal[1]) * (Wy[1] - Scmygoal[1]));
                            if (tmp3 < tmp4)
                            {
                                if (W[i].Id == j)
                                {
                                    cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                }
                                else
                                {
                                    if (Scid[i] == Scmygoal[2])
                                        cout << "MOVE " << Scsecondx[i] << ' ' <<  Scsecondy[i] << " 150" << endl;
                                    else
                                        cout << "MOVE " << Scx[i] << ' ' <<  Scy[i] << " 150" << endl;
                                }

                            }
                            else if (tmp4 <= tmp3)
                            {
                                if (W[i].Id == o)
                                {
                                    cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                }
                                else
                                {
                                    if (Scid[i] == Scmygoal[2])
                                        cout << "MOVE " << Scsecondx[i] << ' ' <<  Scsecondy[i] << " 150" << endl;
                                    else
                                        cout << "MOVE " << Scx[i] << ' ' <<  Scy[i] << " 150" << endl;
                                }
                            }
                        }
                    }
                }
                else
                {
                    //cerr << 1 << endl;
                    double tmp1 = sqrt((Wx[0] - Scx[0]) * (Wx[0] - Scx[0]) + (Wy[0] - Scy[0]) * (Wy[0] - Scy[0]));
                    double tmp2 = sqrt((Wx[1] - Scx[1]) * (Wx[1] - Scx[1]) + (Wy[1] - Scy[1]) * (Wy[1] - Scy[1]));
                    if (W[i].state == 1)
                    {
                        cout << "THROW " << goalx << ' ' << goaly << " 500" << endl;
                    }
                    else if(W[i].state == 0)
                    {
                        if (W[i].myMagic >= 25 && abs(Scmygoal[0] - mygoalx) < 5000)
                        {
                            if (tmp1 < tmp2)
                            {
                                if (W[i].Id == j)
                                {
                                    cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                }
                                else
                                {
                                    if (Scmygoal[1] >= 2050 && Scmygoal[1] <= 5450)
                                    {
                                        if (Scmygoal[1] <= 3750)
                                        {
                                            if (myMagic >= 40)
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << 40 << endl;
                                            else
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << myMagic << endl;
                                        }
                                        else
                                        {
                                            if (myMagic >= 40)
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << 40 << endl;
                                            else
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                            //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                        }
                                    }
                                    else
                                    {
                                        if (myMagic >= 40)
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 40 << endl;
                                        else
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    }
                                }

                            }
                            else if (tmp2 <= tmp1)
                            {
                                if (W[i].Id == o)
                                {
                                    cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                }
                                else
                                {
                                    if (Scmygoal[1] >= 2050 && Scmygoal[1] <= 5450)
                                    {
                                        if (Scmygoal[1] <= 3750)
                                        {
                                            if (myMagic >= 40)
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << 40 << endl;
                                            else
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << myMagic << endl;
                                            //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 7501 << ' ' << myMagic << endl;
                                        }
                                        else
                                        {
                                            if (myMagic >= 40)
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << 40 << endl;
                                            else
                                                cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                            //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                        }
                                    }
                                    else
                                    {
                                        if (myMagic >= 40)
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 40 << endl;
                                        else
                                            cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                        //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    }
                                }
                            }
                        }
                        else if (W[i].myMagic >= 25 && abs(Scgoal[0] - goalx) < 5000)
                        {
                            if (tmp1 < tmp2)
                            {
                                if (W[i].Id == j)
                                {
                                    cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                }
                                else
                                {
                                    if (Scgoal[1] >= 2050 && Scgoal[1] <= 5450)
                                    {
                                        if (Scgoal[1] <= 2900)
                                            cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << 2900 << ' ' << 22 << endl;
                                        else if (Scgoal[1] >= 4600)
                                            cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << 4600 << ' ' << 22 << endl;
                                        else
                                            cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 22 << endl;
                                    }
                                    else
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 22 << endl;
                                }

                            }
                            else if (tmp2 <= tmp1)
                            {
                                if (W[i].Id == o)
                                {
                                    cout << "MOVE " << Scmygoal[0] << ' ' <<  Scmygoal[1] << " 150" << endl;
                                }
                                else
                                {
                                    if (Scgoal[1] >= 2050 && Scgoal[1] <= 5450)
                                    {
                                        if (Scgoal[1] <= 2900)
                                            cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << 2900 << ' ' << 22 << endl;
                                        else if (Scgoal[1] >= 4600)
                                            cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << 4600 << ' ' << 22 << endl;
                                        else
                                            cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 22 << endl;
                                    }
                                    else
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 22 << endl;
                                }
                            }
                        }
                        else
                        {
                            if (Scid[0] == Scid[1])
                            {
                                if (tmp1 < tmp2)
                                {
                                    if (W[i].Id == j)
                                    {
                                        cout << "MOVE " << Scx[i] << ' ' <<  Scy[i] << " 150" << endl;
                                    }
                                    else
                                    {
                                        cout << "MOVE " << Scsecondx[i] << ' ' <<  Scsecondy[i] << " 150" << endl;
                                    }

                                }
                                else if (tmp2 <= tmp1)
                                {
                                    if (W[i].Id == o)
                                    {
                                        cout << "MOVE " << Scx[i] << ' ' <<  Scy[i] << " 150" << endl;
                                    }
                                    else
                                    {
                                        cout << "MOVE " << Scsecondx[i] << ' ' <<  Scsecondy[i] << " 150" << endl;
                                    }
                                }
                            }
                            else
                            {
                                cout << "MOVE " << Scx[i] << ' ' <<  Scy[i] << " 150" << endl;
                            }
                        }
                    }
                }
            }
            else
            {
                if (W[i].state == 1)
                {
                    if (abs(Scx[i] - mygoalx) < 9000)
                    {
                         if (Scy[i] >= 4600)
                            cout << "THROW " << goalx << ' ' << 6000 << " 500" << endl;
                         else if (Scy[i] <= 2900)
                            cout << "THROW " << goalx << ' ' << 1000 << " 500" << endl;
                         else
                            cout << "THROW " << goalx << ' ' << goaly << " 500" << endl;
                    }
                    else
                    {
                        cout << "THROW " << goalx << ' ' << goaly << " 500" << endl;
                    }
                }
                else if (myMagic >= 26)
                {
                    double tmp1 = sqrt((Wx[0] - Scx[0]) * (Wx[0] - Scx[0]) + (Wy[0] - Scy[0]) * (Wy[0] - Scy[0]));
                    double tmp2 = sqrt((Wx[1] - Scx[1]) * (Wx[1] - Scx[1]) + (Wy[1] - Scy[1]) * (Wy[1] - Scy[1]));
                    if (tmp1 < tmp2)
                        {
                            if (W[i].Id == j)
                            {
                                cout << "MOVE " << Scx[0] << ' ' <<  Scy[0] << " 150" << endl;
                            }
                            else
                            {
                                if (centernear[2] == Scid[0])
                                {
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    //cout << "WINGARDIUM " << centernear2[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                }
                                else
                                {

                                    cout << "WINGARDIUM " << centernear[2] << ' ' << goalx << ' ' << Wy[0] << ' ' << myMagic << endl;
                                }
                                //cout << "WINGARDIUM " << centernear[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                            }
                        }
                        else if (tmp2 <= tmp1)
                        {
                            if (W[i].Id == o)
                            {
                                cout << "MOVE " << Scx[1] << ' ' <<  Scy[1] << " 150" << endl;
                            }
                            else
                            {
                                // cerr << centernear[2] << endl;
                                // cerr << Scid[1] << endl;
                                if (centernear[2] == Scid[1])
                                {
                                    //cout << "WINGARDIUM " << centernear2[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                }
                                else
                                {
                                    cout << "WINGARDIUM " << centernear[2] << ' ' << goalx << ' ' << Wy[1] << ' ' << myMagic << endl;
                                }
                                //cout << "WINGARDIUM " << centernear[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                            }
                        }
                }
                else if (myMagic >= 25 && abs(Scmygoal[0] - mygoalx) <= 4000)
                {
                    double tmp1 = sqrt((Wx[0] - Scx[0]) * (Wx[0] - Scx[0]) + (Wy[0] - Scy[0]) * (Wy[0] - Scy[0]));
                    double tmp2 = sqrt((Wx[1] - Scx[1]) * (Wx[1] - Scx[1]) + (Wy[1] - Scy[1]) * (Wy[1] - Scy[1]));
                    if (tmp1 < tmp2)
                        {
                            if (W[i].Id == j)
                            {
                                cout << "MOVE " << Scx[0] << ' ' <<  Scy[0] << " 150" << endl;
                            }
                            else
                            {
                                if (Scmygoal[1] >= 2000 && Scmygoal[1] <= 5000)
                                {
                                    if (Scmygoal[1] <= 3750)
                                        cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 8000 << ' ' << myMagic << endl;
                                    else
                                        cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                }
                                else
                                {
                                    cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                }
                                //cout << "WINGARDIUM " << Scmygoal[2] << ' ' <<  << ' ' << goaly << ' ' << myMagic << endl;
                            }
                        }
                        else if (tmp2 <= tmp1)
                        {
                            if (W[i].Id == o)
                            {
                                cout << "MOVE " << Scx[1] << ' ' <<  Scy[1] << " 150" << endl;
                            }
                            else
                            {
                                //cerr << 1 << endl;
                                if (Scmygoal[1] >= 2000 && Scmygoal[1] <= 5000)
                                {
                                    if (Scmygoal[1] <= 3750)
                                        cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 8000 << ' ' << myMagic << endl;
                                    else
                                        cout << "WINGARDIUM " << Scmygoal[2] << ' ' << 8000 << ' ' << 0 << ' ' << myMagic << endl;
                                }
                                else
                                {
                                    cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                                }
                                //cout << "WINGARDIUM " << centernear[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                            }
                        }
                }
                else if (myMagic >= 12 && abs(Scgoal[0] - goalx) < 500 && (Scgoal[1] >= 2000 && Scgoal[1] <= 5500))
                {
                    double tmp1 = sqrt((Wx[0] - Scx[0]) * (Wx[0] - Scx[0]) + (Wy[0] - Scy[0]) * (Wy[0] - Scy[0]));
                    double tmp2 = sqrt((Wx[1] - Scx[1]) * (Wx[1] - Scx[1]) + (Wy[1] - Scy[1]) * (Wy[1] - Scy[1]));
                    if (tmp1 < tmp2)
                        {
                            if (W[i].Id == j)
                            {
                                cout << "MOVE " << Scx[0] << ' ' <<  Scy[0] << " 150" << endl;
                                //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << magi << endl;
                            }
                            else
                            {
                                if (Scgoal[1] >= 2000 && Scgoal[1] <= 5500)
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 12 << endl;
                                else
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 12 << endl;
                            }
                        }
                        else if (tmp2 <= tmp1)
                        {
                            if (W[i].Id == o)
                            {
                                cout << "MOVE " << Scx[1] << ' ' <<  Scy[1] << " 150" << endl;
                                //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                            }
                            else
                            {
                                if (Scgoal[1] > 2000 && Scgoal[1] < 5500)
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 12 << endl;
                                else
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 12 << endl;
                            }
                        }
                }
                else if (myMagic >= 15 && abs(Scgoal[0] - goalx) < 1500 && Scid[0] != Scgoal[2] && Scid[1] != Scgoal[2])
                {
                    double tmp1 = sqrt((Wx[0] - Scx[0]) * (Wx[0] - Scx[0]) + (Wy[0] - Scy[0]) * (Wy[0] - Scy[0]));
                    double tmp2 = sqrt((Wx[1] - Scx[1]) * (Wx[1] - Scx[1]) + (Wy[1] - Scy[1]) * (Wy[1] - Scy[1]));
                    if (tmp1 < tmp2)
                        {
                            if (W[i].Id == j)
                            {
                                cout << "MOVE " << Scx[0] << ' ' <<  Scy[0] << " 150" << endl;
                                //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << magi << endl;
                            }
                            else
                            {
                                if (Scgoal[1] >= 2200 && Scgoal[1] <= 5300)
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 15 << endl;
                                else
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 15 << endl;
                            }
                        }
                        else if (tmp2 <= tmp1)
                        {
                            if (W[i].Id == o)
                            {
                                cout << "MOVE " << Scx[1] << ' ' <<  Scy[1] << " 150" << endl;
                                //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                            }
                            else
                            {
                                if (Scgoal[1] >= 2200 && Scgoal[1] <= 5300)
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 15 << endl;
                                else
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 15 << endl;
                            }
                        }
                }
                else if (myMagic >= 22 && abs(Scgoal[0] - goalx) < 2000 && Scid[0] != Scgoal[2] && Scid[1] != Scgoal[2])
                {
                    double tmp1 = sqrt((Wx[0] - Scx[0]) * (Wx[0] - Scx[0]) + (Wy[0] - Scy[0]) * (Wy[0] - Scy[0]));
                    double tmp2 = sqrt((Wx[1] - Scx[1]) * (Wx[1] - Scx[1]) + (Wy[1] - Scy[1]) * (Wy[1] - Scy[1]));
                    if (tmp1 < tmp2)
                        {
                            if (W[i].Id == j)
                            {
                                cout << "MOVE " << Scx[0] << ' ' <<  Scy[0] << " 150" << endl;
                                //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << magi << endl;
                            }
                            else
                            {
                                if (Scgoal[1] >= 2050 && Scgoal[1] <= 5450)
                                {
                                    if (Scgoal[1] <= 2900)
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << 2900 << ' ' << 22 << endl;
                                    else if (Scgoal[1] >= 4600)
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << 4600 << ' ' << 22 << endl;
                                    else
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 22 << endl;
                                }
                                else
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 22 << endl;
                            }
                        }
                        else if (tmp2 <= tmp1)
                        {
                            if (W[i].Id == o)
                            {
                                cout << "MOVE " << Scx[1] << ' ' <<  Scy[1] << " 150" << endl;
                                //cout << "WINGARDIUM " << Scmygoal[2] << ' ' << goalx << ' ' << goaly << ' ' << myMagic << endl;
                            }
                            else
                            {
                                if (Scgoal[1] >= 2050 && Scgoal[1] <= 5450)
                                {
                                    if (Scgoal[1] <= 2900)
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << 2900 << ' ' << 22 << endl;
                                    else if (Scgoal[1] >= 4600)
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << 4600 << ' ' << 22 << endl;
                                    else
                                        cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << Scgoal[1] << ' ' << 22 << endl;
                                }
                                else
                                    cout << "WINGARDIUM " << Scgoal[2] << ' ' << goalx << ' ' << goaly << ' ' << 22 << endl;
                            }
                        }
                }
                else
                {
                    double tmp1 = sqrt((Wx[0] - Scx[0]) * (Wx[0] - Scx[0]) + (Wy[0] - Scy[0]) * (Wy[0] - Scy[0]));
                    double tmp2 = sqrt((Wx[1] - Scx[1]) * (Wx[1] - Scx[1]) + (Wy[1] - Scy[1]) * (Wy[1] - Scy[1]));
                    if (Scid[0] == Scid[1] && snaffle != 1)
                    {
                        //cerr << Scsecondid[i] << endl;
                        if (tmp1 < tmp2)
                        {
                            if (W[i].Id == j)
                            {
                                cout << "MOVE " << Scx[0] << ' ' <<  Scy[0] << " 150" << endl;
                                //cerr << Scid[i] << endl;
                            }
                            else
                            {
                                if (Scsecondid[0] == Scsecondid[1])
                                {
                                    //cerr << 2 << endl;
                                    double tmp3 = sqrt((Wx[0] - Scsecondx[0]) * (Wx[0] - Scsecondx[0]) + (Wy[0] - Scsecondy[0]) * (Wy[0] - Scsecondy[0]));
                                    double tmp4 = sqrt((Wx[1] - Scsecondx[1]) * (Wx[1] - Scsecondx[1]) + (Wy[1] - Scsecondy[1]) * (Wy[1] - Scsecondy[1]));
                                    if (tmp3 > tmp4)
                                        cout << "MOVE " << Scsecondx[1] << ' ' <<  Scsecondy[1] << " 150" << endl;
                                    else
                                    {
                                        cout << "MOVE " << Scthirdx[1] << ' ' <<  Scthirdy[1] << " 150" << endl;
                                        //cerr << 1 << endl;
                                    }
                                }
                                else
                                {
                                    cout << "MOVE " << Scsecondx[1] << ' ' <<  Scsecondy[1] << " 150" << endl;
                                }
                            }
                        }
                        else if (tmp2 <= tmp1)
                        {
                            if (W[i].Id == o)
                            {
                                cout << "MOVE " << Scx[1] << ' ' <<  Scy[1] << " 150" << endl;
                                //cerr << Scid[i] << endl;
                            }
                            else
                            {
                                if (Scsecondid[0] == Scsecondid[1])
                                {
                                    //cerr << 3 << endl;
                                    double tmp3 = sqrt((Wx[0] - Scsecondx[0]) * (Wx[0] - Scsecondx[0]) + (Wy[0] - Scsecondy[0]) * (Wy[0] - Scsecondy[0]));
                                    double tmp4 = sqrt((Wx[1] - Scsecondx[1]) * (Wx[1] - Scsecondx[1]) + (Wy[1] - Scsecondy[1]) * (Wy[1] - Scsecondy[1]));
                                    if (tmp3 < tmp4)
                                        cout << "MOVE " << Scsecondx[0] << ' ' <<  Scsecondy[0] << " 150" << endl;
                                    else
                                    {
                                        cout << "MOVE " << Scthirdx[0] << ' ' <<  Scthirdy[0] << " 150" << endl;
                                    }
                                }
                                else
                                {
                                    cout << "MOVE " << Scsecondx[0] << ' ' <<  Scsecondy[0] << " 150" << endl;
                                }
                            }
                        }
                    }
                    else
                    {
                        cout << "MOVE " << Scx[i] << ' ' <<  Scy[i] << " 150" << endl;
                    }
                }
            }
        }
    }
}
