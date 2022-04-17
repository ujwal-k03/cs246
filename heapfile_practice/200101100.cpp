#include <bits/stdc++.h>

using namespace std;

struct page
{   
    int* primary_keys;
    int count;
    page* prev_page;
    page* next_page;
    int free_index;
    int occupied_space = 16;

    int* pointers;
};

class heap_file
{
    page* root_page;
    page* latest_page;
    int no_pages;
    int size_page;

    public:
        void init(int huge)
        {
            
            size_page = huge;
            root_page = NULL;
            latest_page = NULL;
            no_pages = 0;
            
        }

        page* create_page()
        {
            page* new_page = new page;
            new_page->primary_keys = (int*)malloc(size_page*(sizeof(int)));
            new_page->count = 0;
            new_page->prev_page = NULL;
            new_page->next_page = NULL;
            new_page->free_index = 0;
            new_page->pointers = (int*)malloc(size_page*(sizeof(int)));
            new_page->pointers[0]=0;

            no_pages++;

            return new_page;

            
        }

        page* search_spot(int r_size)
        {   
            if(root_page == NULL)
            {   
                
                page* new_root = create_page();
                root_page = new_root;
                latest_page = new_root;
            }

            
            page* curr_page = root_page;
            
            
            while(((size_page-(curr_page->occupied_space)) < (r_size+4)))
            {   
                curr_page = curr_page->next_page;
                if(curr_page == NULL)
                    break;
            }
            

            if(curr_page == NULL)
            {
                page* new_latest = create_page();
                curr_page = new_latest;
                latest_page->next_page = new_latest;
                new_latest->prev_page = latest_page;
                latest_page = new_latest;
            }
            
            return curr_page;
        }

        void insert(int r_size, int val)
        {   
            
            page* page_to_insert = search_spot(r_size);

            int occ = r_size + 4;
            (page_to_insert->occupied_space)+= occ;
            
            
            page_to_insert->primary_keys[(page_to_insert->count)] = val;
            
            
            (page_to_insert->count)++;
            

            (page_to_insert->pointers[page_to_insert->count]) = (page_to_insert->pointers[(page_to_insert->count) - 1]) + r_size;
            page_to_insert->free_index = page_to_insert->pointers[page_to_insert->count];
        }


        void search(int val)
        {
            page* currpage = root_page;
            
            int page_id = -1, slot_id =-1;
            int fake_page_id = 0;
            while(currpage != NULL)
            {
                for(int i=0; i<(currpage->count); i++)
                {
                    if(currpage->primary_keys[i] == val)
                    {
                        slot_id = i;
                        page_id = fake_page_id;
                        break;
                    }
                }

                if(slot_id != -1)
                    break;

                currpage = currpage->next_page;
                fake_page_id++;

            }

            cout<<page_id<<" "<<slot_id<<"\n";
        }

        void status()
        {
            cout<<no_pages;
            
            page* currpage = root_page;

            while(currpage != NULL)
            {
                cout<<" "<<currpage->count;

                currpage = currpage->next_page;
            }

            cout<<"\n";
        }



        void show_all()
        {   

            page* currpage = root_page;
            
            while(currpage != NULL)
            {   

                show(currpage);
                currpage = currpage->next_page;
            }
        }

        void show(page* currpage)
        {
            cout<<"----------\n";
            cout<<"Count = "<<currpage->count<<"\n";
            cout<<"Occupied Space = "<<currpage->occupied_space<<"\n";
            cout<<"Pointers: ";
            for(int i=0; i<(currpage->count); i++)
            {
                cout<<currpage->pointers[i]<<" ";
            }
            cout<<"\n";
            cout<<"Primary keys: ";
            for(int i=0; i<(currpage->count); i++)
            {
                cout<<currpage->primary_keys[i]<<" ";
            }
            cout<<"\n";
            cout<<"Free index = "<<currpage->free_index<<"\n";
        }
};


int main()
{   
    heap_file epic;

    int size;
    cin>>size;

    epic.init(size);

    int command;
    cin>>command;
    while(command!=4)
    {
        if(command == 1)
        {   
            int op1,op2;
            cin>>op1>>op2;
            epic.insert(op1,op2);
        }
        if(command == 2)
        {
            epic.status();
        }
        if(command == 3)
        {
            int op1;
            cin>>op1;
            epic.search(op1);
        }
        cin>>command;
    }
}