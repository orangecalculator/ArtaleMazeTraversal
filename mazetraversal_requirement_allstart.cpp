#include <iostream>
#include <map>
#include <queue>
#include <vector>

constexpr unsigned INF = 0x100000;

using namespace std;


typedef struct { int req, location; } dim_loc;
typedef struct _policy{    dim_loc cur, next; int move, dist; } policy;

int next_room(int room, int move){
    // forward step
    if(room==16) return (move==2 ? 9 : 16);
    else if(room==9 && move==2) return 16;
    else if(move==1) return (room+3)%15+1;
    else if(move==2) return (room+6)%15+1;
    else return (room+11)%15+1;
}

int inv_next_room(int room,int move){
    if(room==16) return (move==2 ? 9 : 16);
    else if(move==1) return (room+10)%15+1;
    else if(move==2) return (room!=1 ?(room+7)%15+1 : 1);
    else return (room+2)%15+1;
}

char move_decode(int move){
    if(move == 1) return 'L';
    else if(move == 2) return 'M';
    else if(move == 3) return 'R';
    else return ' ';
}

int main() {
    
    int dest;
    int req = 0;
    vector<bool> required = vector<bool>(17,false);
    
    dest = 16;
    for(int buf;cin>>buf;){
        req |= (1<<buf);
        required[buf] = true;
    }
    
    req &= ~(1<<dest);
    
    // BFS based search
    queue<policy> roomtrace;
    map<pair<int,int>,policy> mem;
    
    vector<int> found = vector<int>(17,0);
    int found_count = 0;
    
    // [{ shortage encoding, current location}]
    {
        mem[{0,0}] = {0,0,0,0,0,0};
        policy last_move = {{1<<dest,dest},{0,0},0,0};
        mem[{1<<dest,dest}] = last_move;
        roomtrace.push(last_move);
    }
    
    while(!roomtrace.empty()){
        auto current = roomtrace.front();
        roomtrace.pop();
        
        if((current.cur.req & req) == req){
            if(found[current.cur.location] == 0){
                found[current.cur.location] = current.cur.req;
                found_count++;
                //printf("found %2d\n",current.cur.location);
                if(found_count == 16) break;
            }
            
            
        }
        
        for(int move=1;move<=3;++move){
            int last = inv_next_room(current.cur.location,move);
            int last_req = current.cur.req | (1<<last);
            if(mem.find({last_req,last}) == mem.end()){
                policy last_policy = {{last_req,last},current.cur,move,current.dist+1};
                mem[{last_req,last}] = last_policy;
                roomtrace.push(last_policy);
            }
            
            if(current.cur.location == 9 && move == 2){
                int last = 16;
                int last_req = current.cur.req | (1<<last);
                if(mem.find({last_req,last}) == mem.end()){
                    policy last_policy = {{last_req,last},current.cur,move,current.dist+1};
                    mem[{last_req,last}] = last_policy;
                    roomtrace.push(last_policy);
                }
            }
        }
    }
    
    for(int start=1;start<=16;++start){
        printf("%c %2d",required[start] ? '*' : ' ',start);
        for(policy next = mem[{found[start],start}];
                next.next.location!=0;next = mem[{next.next.req,next.next.location}])
            printf(" %c -> %c %2d",move_decode(next.move),required[next.next.location] ? '*' : ' ',next.next.location);
        printf("\n");
    }
    
}