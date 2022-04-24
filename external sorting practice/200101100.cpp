#include <bits/stdc++.h>

using namespace std;

class external_sorting
{   
    public:

    int rec_per_page,buf_pages,total_records;
    int* file_data;
    int* buffer;

    void init(int b,int r,int n)
    {   
        rec_per_page = r;
        buf_pages = b;
        total_records = n;
        buffer = (int*)malloc((r*b)*sizeof(int));
    }

    void give_data(int* data_pointer)
    {
        file_data = data_pointer;
    }

    void status_custom()
    {
        for(int i=0; i<total_records; i++)
        {
            cout<<file_data[i]<<"\n";
        }
    }
    
    void ext_sort()
    {
        int pass=0;
        int run_size=0;
        
        // PASS = 0
        for(int i=0; i<total_records; i=i+(buf_pages*rec_per_page)) 
        {   
            sort(file_data+i,file_data+min(total_records,(i+(buf_pages*rec_per_page))));
        }
        
        // cout<<"------------- PASS 0 -------------- \n";
        // status_custom();

        pass++;
        run_size=(buf_pages*rec_per_page); // run_size = b;

        while(run_size < total_records)
        {
            for(int i=0; i<total_records; i = i + ((buf_pages-1)*run_size))
            {
                sort(file_data+i,file_data+min(total_records,(i + ((buf_pages-1)*run_size))));
            }
            
            // cout<<"--------------"<<" PASS "<<pass<<" ---------------\n";
            // status_custom();
            run_size = run_size*(buf_pages-1);
            pass++;
        }

    }
};

int main()
{
    external_sorting epic;

    int b;
    int r;
    int n;

    cin>>b>>r>>n;

    int data[n];
    epic.init(b,r,n);

    int temp;
    for(int i=0; i<n; i++)
    {   
        cin>>temp;
        data[i] = temp;
    }
    

    epic.give_data(data);
    epic.ext_sort();

    // cout<<"------------- FINAL RESULT ----------------\n";
    epic.status_custom();

    

}