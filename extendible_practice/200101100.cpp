#include <bits/stdc++.h>

using namespace std;

struct bucket
{   
    int current_capacity;
    int local_depth;
    bool is_checked;

    int* values;

};

class directory
{
    private:
        int global_depth=0;
        int bucket_capacity=0;
        int nb;
        vector<bucket *> arr;
    
    public:
        int return_gd()
        {
            return global_depth;
        }

        int return_bc()
        {
            return bucket_capacity;
        }

        void init(int gd, int bc)
        {
            global_depth = gd;
            bucket_capacity = bc;
            nb = (1<<gd);

            for(int i=0; i<(1<<gd); i++)
            {   
                arr.push_back((new bucket));
                arr[i]->current_capacity = 0;
                arr[i]->local_depth = gd;
                arr[i]->values = (int *)malloc(bucket_capacity*sizeof(int));
            }
            
        }

        void insert(int key)
        {   
            int index;
            index = key%(1<<global_depth);

            if(arr[index]->current_capacity == bucket_capacity)
            {
                if(arr[index]->local_depth < global_depth)
                {
                    split_bucket(index);
                    insert(key);
                }
                else
                {   
                    extend_dir();
                    insert(key);
                }
            
            } 
            else
            {
                arr[index]->values[(arr[index]->current_capacity)] = key;
                (arr[index]->current_capacity)++;

            }
        }

        

        void split_bucket(int index)
        {   
            nb++;
            bucket* curr_bucket = arr[index]; 

            bucket* bucket0 = new bucket;
            bucket* bucket1 = new bucket;

            int ld = (arr[index]->local_depth);

            bucket0->local_depth = ld+1;
            bucket1->local_depth = ld+1;

            bucket0->current_capacity = 0;
            bucket1->current_capacity = 0;

            bucket0->values = (int *)malloc(bucket_capacity*sizeof(int));
            bucket1->values = (int *)malloc(bucket_capacity*sizeof(int));

            for(int i=0; i<(1<<global_depth); i++)
            {
                if(arr[i] == curr_bucket)
                {
                    if((1<<ld)&i)
                    {
                        arr[i]=bucket1;
                    }
                    else
                        arr[i]=bucket0;
                }
            }

            for(int i=0; i<bucket_capacity; i++)
            {
                insert(curr_bucket->values[i]);
            }

        }

        void extend_dir()
        {

            for(int i=0; i<(1<<global_depth); i++)
            {   
                arr.push_back(arr[i]);
            }

            global_depth++;
        }

        void delete_key(int key)
        {   
            bool isfound = false;
            int found_pos;
            int index = key%(1<<global_depth);
            for(int i=0; i<bucket_capacity; i++)
            {   
                if(arr[index]->values[i] == key)
                {   
                    isfound = true;
                    found_pos = i;
                    (arr[index]->current_capacity)--;
                    break;
                }
            }

            if(isfound)
            {
                for(int i=found_pos; i<arr[index]->current_capacity; i++)
                {
                    arr[index]->values[i] = arr[index]->values[i+1];
                }
            }
            
        }

        int search_key(int key)
        {
            bool isfound = false;
            int found_pos = -1;
            int index = key%(1<<global_depth);
            for(int i=0; i<bucket_capacity; i++)
            {   
                if(arr[index]->values[i] == key)
                {   
                    isfound = true;
                    found_pos = i;
                    break;
                }
            }

            return found_pos;
        }
        void status()
        {
            cout<<global_depth<<"\n";
            cout<<nb<<"\n";

            for(int i=0; i<(1<<global_depth); i++)
            {
                arr[i]->is_checked = false;
            }

            for(int i=0; i<(1<<global_depth); i++)
            {
                if(!(arr[i]->is_checked))
                {
                    arr[i]->is_checked = true;
                    cout<<arr[i]->current_capacity<<" "<<arr[i]->local_depth<<"\n";
                }
            }
        }

};


int main()
{
    int gd;
    int bc;

    cin>>gd;
    cin>>bc;

    directory epic;

    epic.init(gd,bc);

    int command, operand;
    while(1)
    {
        cin>>command;

        if((command!=5)&&(command!=6))
            cin>>operand;

        switch (command)
        {
            case 2: epic.insert(operand); break;
            case 3: epic.search_key(operand); break;
            case 4: epic.delete_key(operand); break;
            case 5: epic.status(); break;
            case 6: return 0;
            default: break;
        }

    }
}