
#include <stdio.h>
#include <stdlib.h>

#include<string.h>
#include<math.h>

#define amp 5
#define tmax amp-1
#define tmin 2//its a double value
#define ampp 5
#define dmax ampp-1
#define dmin 2

typedef struct keyy
{ int roll;
char sub_code[10];
}keyy;

typedef struct stu_reco
{ int marks, sem;
   struct keyy *key;
  char name[10], dept[10];
}rec_node;

typedef struct rec_ptrr
{struct stu_reco *ptr;
}rec_ptrr;

typedef struct dataa
{ struct rec_ptrr rec_ptr[dmax];
  struct dataa *next;
  struct dataa *prev;
  struct treee *parent;
}data_node;

union bptrr
{ struct dataa *d_ptr;
  struct treee *t_ptr;
};



typedef struct keey
{struct keyy *key_ptr;
}keey;

typedef struct treee
{ struct keey key[tmax];
  union bptrr bptr[tmax+1];
  int c;// TO RESTRICT USE TO POINT ONLY 1 TYPE OF NODES---> SET THIS TO '0' FOR POINTING DATAA NODES AND '1' FOR POINTING TREE NODES; AND WHILE IMPLEMENTING ALWAYS CHECK THIS VALUE BEFORE WRITING ANY FURTHURE CODE,& ALWAYS  UPDATE THIS VALUE :)
   struct treee *parent;
}tree_node;
int which_ptr(tree_node *trec, keyy*key1);



data_node *list_ptr=NULL;

//functions

void print()
{ printf("\n");
  data_node *temp=list_ptr; int i,done;
  while(temp!=NULL)
  { i=0,done=0;
    while(i<dmax &&!done)
    { if(temp->rec_ptr[i].ptr!=NULL)
      { printf("%d",temp->rec_ptr[i].ptr->key->roll);
        printf("%s",temp->rec_ptr[i].ptr->key->sub_code); i++;
      }
      else
      { done=1; }
    }
    printf("\t");temp=temp->next;
  }
}

int equals(keyy *key1,keyy *key2)//0 if same....... 1 if key1 is greter..... -1 if key2 is greater
{ if(key1!=NULL && key2!=NULL)
  { if( key1->roll > key2->roll)
    { return (1);}
    else if(key1->roll < key2->roll)
    { return (-1);}
    else
    { if(strcmp(key1->sub_code,key2->sub_code)<0)
      { return (-1);}
      else if(strcmp(key1->sub_code,key2->sub_code)>0)
      { return (1);}
      else
      { return (0); }
    }
  }
  else if(key1==NULL && key2 !=NULL)
  { return(-1); }//check re
  else
  { printf("error no key to compare"); }
}

int which_ptr(tree_node *trec, keyy*key1)
{ int ret_val,done=0,res,i;
  for( i=0;i<tmax &&!done;i++)
  { if(trec->key[i].key_ptr!=NULL)
    { res=equals(trec->key[i].key_ptr,key1);
      if (res>0)
      {ret_val=i;done=1;}
      else if(res==0)
      { ret_val=i+1; done=1;}
    }
    else
    { ret_val=i;done=1; }
  }
  if(!done)
  { ret_val=tmax;}
  return(ret_val);
}
int t_has_space(tree_node *trec)//1=>has space....0=> nope
{ int done=0,i;
      for(i=0;i<tmax && !done;i++)
      { if(trec->key[i].key_ptr==NULL)
        { done=1; }
      }
      if(done)
      { return(1); }
      else if(!done)
      { return(0); }
}

tree_node* treeAdjusting(tree_node *root,tree_node *trec, tree_node *new_trec,keyy *key1)//sending old tree_node ka parentas parent @c=1
{/*printf("in t_juggad");*/int i; keyy *key2;
  if(trec==NULL)
  { tree_node *trec2;
    trec2= (tree_node*)malloc(sizeof(tree_node));
    trec2->key[0].key_ptr= key1;
    trec2->c=1;
    trec2->bptr[0].t_ptr=root;
    trec2->bptr[1].t_ptr=new_trec;
    for(i=1;i<tmax;i++)
    {  trec2->key[i].key_ptr=NULL;}
    for(i=2;i<tmax;i++)
    { trec2->bptr[i].t_ptr=NULL;}
    root->parent=trec2;
    new_trec->parent=trec2;
    root= trec2;
  }
  else if(t_has_space(trec)==1)//seee
  { int j= which_ptr(trec,key1),i;
    for(i=tmax-1;i>j;i--)
    { trec->key[i].key_ptr= trec->key[i-1].key_ptr;}
    for(i=tmax;i>j;i--)
    { trec->bptr[i].t_ptr= trec->bptr[i-1].t_ptr; }
    trec->key[j].key_ptr=key1;
    trec->bptr[j+1].t_ptr=new_trec;
    new_trec->parent=trec;
  }
  else
  { //call  resursively
    //create another tree_node adjust everything at this level n call recursively
    tree_node *trec2; int j;
    trec2= (tree_node*)malloc(sizeof(tree_node));
    trec2->c=1;
    for(i=0;i<tmax;i++)
    {  trec2->key[i].key_ptr=NULL;}
    for(i=0;i<tmax;i++)
    { trec2->bptr[i].t_ptr=NULL;}
    key2=trec->key[tmin].key_ptr;
    trec->key[tmin].key_ptr=NULL;//dint gusofy key1
    for(i=tmin+1,j=0;i<tmax;i++)
    { trec2->key[j].key_ptr=trec->key[i].key_ptr;
      trec->key[i].key_ptr=NULL;
    }
    for(i=tmin+1,j=0;i<tmax;i++)
    { trec2->bptr[j].t_ptr=trec->bptr[i].t_ptr;
      trec->bptr[i].t_ptr=NULL;
    }
    for(i=0;i<=tmax && trec2->bptr[i].t_ptr!=NULL;i++)
    { trec2->bptr[i].t_ptr->parent=trec2; }
    root= treeAdjusting(root,trec->parent,trec2,key2);
  }
  return(root);
}



data_node* Adjusting(tree_node **root,tree_node *trec, data_node *drec, keyy *key1)
{ data_node *drec2, *ret_drec,*d_temp; keyy *key2,*key3=NULL;//k2 to be inserted in trec
  int j= which_ptr(trec,key1),i,temp;
  if( t_has_space(trec)==1)
  { drec2=(data_node*)malloc(sizeof(data_node)); //printf("juggad called here\n");
          for(i=0;i<dmax;i++)
          { drec2->rec_ptr[i].ptr=NULL; }
          if(drec==list_ptr){list_ptr=drec2;}
          d_temp=drec->prev;
          if(d_temp!=NULL)
         { d_temp->next=drec2;}
          drec2->prev=d_temp;
          drec2->next=drec;
          drec->prev=drec2;
          drec2->parent=trec;
          int temp;
          for(i=0;i<dmin;i++)
          { drec2->rec_ptr[i].ptr=drec->rec_ptr[i].ptr;}
          for(temp=0;i<dmax;temp++,i++)
          { drec->rec_ptr[temp].ptr=drec->rec_ptr[i].ptr;
            drec->rec_ptr[i].ptr=NULL;}
          for(i=tmax-1;i>j;i--)
          { trec->key[i].key_ptr= trec->key[i-1].key_ptr;}
          for(i=tmax;i>j;i--)
          { trec->bptr[i].d_ptr= trec->bptr[i-1].d_ptr; } //printf("current %d j=%d",drec->rec_ptr[0].ptr->key->roll,j);
          trec->key[j].key_ptr=drec->rec_ptr[0].ptr->key;
          trec->bptr[j].d_ptr=drec2;
           
          if(equals(trec->key[j].key_ptr,key1)<0)
          { return(drec);}//check
          else
          { return(drec2);}
  }
  else
  {       drec2=(data_node*)malloc(sizeof(data_node));
          for(i=0;i<dmax;i++)
          { drec2->rec_ptr[i].ptr=NULL; }
          if(drec==list_ptr){list_ptr=drec2;}
          d_temp=drec->prev;
          if(d_temp!=NULL){d_temp->next=drec2;}
          drec->prev=drec2;
          drec2->prev=d_temp;
          drec2->next=drec;     //yet to set parent
          for(i=0;i<dmin;i++)
          { drec2->rec_ptr[i].ptr=drec->rec_ptr[i].ptr;}
          for(temp=0;i<dmax;temp++,i++)
          { drec->rec_ptr[temp].ptr=drec->rec_ptr[i].ptr;
            drec->rec_ptr[i].ptr=NULL;}
          key2=drec->rec_ptr[0].ptr->key;
          if(equals(drec->rec_ptr[0].ptr->key,key1)<0)
          { ret_drec=drec;}//check
          else
          { ret_drec=drec2;}

    tree_node *trec2;
    trec2=(tree_node*)malloc(sizeof(tree_node));
    trec2->c=0;
    int i=tmax-1,temp=tmin-1,done=0;
    while(!done)
    { if(equals(trec->key[i].key_ptr,key2)>0)
      { trec2->key[temp].key_ptr=trec->key[i].key_ptr;
        trec->key[i].key_ptr=NULL;
        i--; temp--;
      }
      else
      { trec2->key[temp].key_ptr=key2;
        temp--; done=2;
      }
      if( !done && temp==-1)
      { done=1; }
    }
    while(temp>-1)
    { trec2->key[temp].key_ptr=trec->key[i].key_ptr;
      trec->key[i].key_ptr=NULL;
      i--; temp--;
    }
    if(done!=2 && equals(trec2->key[i].key_ptr,key2)>0)
    {key3=trec->key[i].key_ptr;//to send to tree above
    trec->key[i].key_ptr=NULL;
    i--; j=i;}
    else if(done==2)
   { key3=trec->key[i].key_ptr;
     trec->key[i].key_ptr=NULL;
    i--; j=i;}
    else if( equals(trec2->key[i].key_ptr,key2)<0)
    {  key3=key2;j=i;}
    int k;done=0;

    for(i=0;i<tmin &&!done;i++)
    { if(equals(trec2->key[i].key_ptr,key2)==0)
      { k=i; done=1; }
    }
    if(done)
    { temp=tmin; i=tmax;
      while(temp!=k)
      { trec2->bptr[temp].d_ptr= trec->bptr[i].d_ptr;
        trec->bptr[i].d_ptr=NULL;
        i--; temp--;
      }
      trec2->bptr[temp].d_ptr=drec2; drec2->parent= trec2;
      temp--;
      while(temp!=-1)
      { trec2->bptr[temp].d_ptr=trec->bptr[i].d_ptr;
        trec->bptr[i].d_ptr=NULL;
        i--; temp--;
      }
      for(i=0;i<tmax && trec2->bptr[i].d_ptr!=NULL;i++)
      {
          trec2->bptr[i].d_ptr->parent=trec2;
      }
    }
    else
    { done=0;
      for(i=0;i<tmax &&!done;i++)
      { if(equals(trec->key[i].key_ptr,key2)==0)
        { k=i; done=1; }
      }
      if(done)
      { i=tmax,temp=tmin;
        while(temp!=-1)
        { trec2->bptr[temp].d_ptr=trec->bptr[i].d_ptr;
          trec->bptr[i].d_ptr=NULL;
          i--; temp--;
        }
        while(i!=k)
        { trec->bptr[i].d_ptr=trec->bptr[i-1].d_ptr;
          i--;
        }
        trec->bptr[i].d_ptr= drec2; drec2->parent=trec;
        i--;
      }
      else// in key sent above
      { for(i=j+1,temp=0;i<=tmax;i++,temp++)
        { trec2->bptr[temp].d_ptr=trec->bptr[i].d_ptr;
          trec->bptr[i].d_ptr=NULL;
        }
        trec->bptr[j].d_ptr= drec2; drec2->parent=trec;
      }
    }
if(key3!=NULL)
  { *root=treeAdjusting(*root,trec->parent,trec2,key3); }//printf("corious %d is root\n",(*root)->key[0].key_ptr->roll);}//to adjust in parent @c=1
   return ret_drec;
 }
}



int has_space(data_node *drec)
{ int done=0,i;
  if(drec==NULL)
  { return(-2); }
  else
  { for(i=0;i<dmax && !done;i++)
    { if(drec->rec_ptr[i].ptr==NULL)
      { done=1; }
    }
    if(i<=dmin)
    { return(0); }
    else if(i<dmax)
    { return(1); }
    else if(!done)
    { return(-1); }
  }
}
int present(data_node *drec, keyy *key1)
{ int i=0,done=0;
 for(i=0;i<dmax && !done;i++)
 { if(drec->rec_ptr[i].ptr!=NULL)
   { if(equals(drec->rec_ptr[i].ptr->key,key1)==0)
     { return (1); done=1; }
     //else if(equals(drec->rec_ptr[i].ptr->key,key1)>0)
   //{ return (0); done=1; }
   }
   else
   { return(0); }
 }
 if(!done)
 { return(0); }
}

data_node* insearch(tree_node **root,keyy *key1)
{ tree_node *trec=*root, *trec1; int done=0,i,res,j; data_node *drec, *drec2; rec_node *stu_temp;
  while( trec->c==1)
  { done=0;//printf("oye");
    for(i=0;i<tmax && !done;i++)
    { if(trec->key[i].key_ptr!=NULL)
       { res=equals(trec->key[i].key_ptr,key1);
         if( res>0)
         { trec=trec->bptr[i].t_ptr;
           done=1;
         }
         else if(res==0)
         { trec=trec->bptr[i+1].t_ptr;
           done=1;
         }
       }
       else
       { trec=trec->bptr[i].t_ptr; done=1;}
    }
    if(!done)
    { trec=trec->bptr[tmax].t_ptr; }
  }
  done=0;
  for(i=0;i<tmax  && !done;i++)//here c=0
  { if(trec->key[i].key_ptr!=NULL)
    { if(equals(trec->key[i].key_ptr,key1)>0)
      { drec=trec->bptr[i].d_ptr; done=1;j=i;}
      else if(equals(trec->key[i].key_ptr,key1)==0)
      { drec=trec->bptr[i+1].d_ptr; done=1; j=i+1;}
    }
    else
    { drec=trec->bptr[i].d_ptr; done=1;j=i;}
  }
  //if(drec->rec_ptr[0].ptr!=NULL)
   //{printf(" spl if%d \n",drec->rec_ptr[0].ptr->key->roll);}
  if(!done)
  { j=tmax; drec=trec->bptr[j].d_ptr;}
  done=0;
  if(drec!=NULL && has_space(drec)>=0)// || has_space(drec)==-1)//0 if less than min.... 1->can insert.... -1 if already full...... -2 if null
  { return (drec);}
  else if(drec==NULL)//initially only
  { drec=(data_node*)malloc(sizeof(data_node)); //printf("made new node");
    for(i=0;i<dmax;i++)
    { drec->rec_ptr[i].ptr=NULL; }
    list_ptr->prev=drec;
    drec->next=list_ptr;
    drec->prev=NULL;
    list_ptr=drec;
    trec->bptr[j].d_ptr=drec;
    drec->parent=trec;
    return(drec);
  }
  else if(has_space(drec)==-1)//data_record is full
  {// printf("exp case is %d",trec->key[0].key_ptr->roll);
    if(j<tmax && j>0)
    { drec2=trec->bptr[j+1].d_ptr;
      if(has_space(drec2)==1 || has_space(drec2)==0)
      { trec->key[j].key_ptr=key1; done=1;
        return(drec2);
      }
      else
      {
        drec2=trec->bptr[j-1].d_ptr;
        if(has_space(drec2)==1 || has_space(drec2)==0)
        { stu_temp= drec->rec_ptr[0].ptr;
          for(i=0;i<dmax-1;i++)
          { drec->rec_ptr[i].ptr= drec->rec_ptr[i+1].ptr; }
          drec->rec_ptr[i].ptr=NULL;
          if(equals(drec->rec_ptr[0].ptr->key,key1)<0)
          { trec->key[j-1].key_ptr= drec->rec_ptr[0].ptr->key;}
          else
          { trec->key[j-1].key_ptr= key1;}
          done=0;
          for(i=0;i<dmax && !done;i++)
          { if (drec2->rec_ptr[i].ptr==NULL)
            { drec2->rec_ptr[i].ptr=stu_temp;
              done=1;
              return(drec);
            }
          }
        }
        else if(has_space(drec2)==-2)//if left is null..... initial condition
        { drec2=(data_node*)malloc(sizeof(data_node)); data_node *d_temp;
          for(i=0;i<dmax;i++)
          { drec2->rec_ptr[i].ptr=NULL; }
          if(drec==list_ptr){list_ptr=drec2;}
          d_temp= drec->prev;
          if(d_temp!=NULL) {d_temp->next=drec2; }
          drec->prev=drec2;
          drec2->prev=d_temp;
          drec2->next=drec;
          drec2->parent=trec; int temp;
          for(i=0;i<dmin;i++)
          { drec2->rec_ptr[i].ptr=drec->rec_ptr[i].ptr; drec->rec_ptr[i].ptr=NULL; }
          for(temp=0;i<dmax;temp++,i++)
          { drec->rec_ptr[temp].ptr=drec->rec_ptr[i].ptr; drec->rec_ptr[i].ptr=NULL;}
          trec->key[j-1].key_ptr=drec->rec_ptr[0].ptr->key;
          trec->bptr[j-1].d_ptr= drec2;
          if(equals(trec->key[j-1].key_ptr,key1)<0)
          { return(drec); done=1;}
          else
          { return(drec2); done=1;}

        }
        else//all full
        { trec1=*root; drec2=Adjusting(&trec1,trec,drec,key1); done=1;
          *root=trec1;
          return( drec2);
        }
      }
     }
     else if(j==tmax)
      { drec2=trec->bptr[j-1].d_ptr;
        if(has_space(drec2)==1 || has_space(drec2)==0)
        { stu_temp= drec->rec_ptr[0].ptr;
          for(i=0;i<dmax-1;i++)
          { drec->rec_ptr[i].ptr= drec->rec_ptr[i+1].ptr; }
          drec->rec_ptr[i].ptr=NULL;
          if(equals(drec->rec_ptr[0].ptr->key,key1)<0)
          { trec->key[j-1].key_ptr= drec->rec_ptr[0].ptr->key;}
          else
          { trec->key[j-1].key_ptr= key1;}
          done=0;
          for(i=0;i<dmax && !done;i++)
          { if (drec2->rec_ptr[i].ptr==NULL)
            { drec2->rec_ptr[i].ptr=stu_temp;
              done=1;
              return(drec);
            }
          }
        }
        else//all full
        {  trec1=*root; drec2=Adjusting(&trec1,trec,drec,key1); done=1;
           *root=trec1;
           return( drec2);
        }
      }
      else if(j==0)
      { drec2=trec->bptr[j+1].d_ptr;
        if(has_space(drec2)==1 || has_space(drec2)==0)
        { trec->key[j].key_ptr=key1; done=1;
          return(drec2);
        }
        else//all full
        {  trec1=*root; drec2=Adjusting(&trec1,trec,drec,key1); done=1;
           *root=trec1;
           return( drec2);
        }
      }
   }

}


tree_node* insert(tree_node *root, rec_node *rec)
{
 if(list_ptr==NULL && root == NULL)
 { data_node *drec;
   drec=(data_node*)malloc(sizeof(data_node));
   drec->next=NULL;
   drec->prev=NULL;
   drec->parent=NULL;
   drec->rec_ptr[0].ptr=rec;
   int i;
   for(i=1;i<dmax;i++)
   { drec->rec_ptr[i].ptr=NULL;}
   list_ptr=drec; //print();
   if(dmin==1)
   {  tree_node *trec;
      trec=(tree_node*)malloc(sizeof(tree_node));
      trec->parent=NULL;
      root=trec;
      trec->key[0].key_ptr=rec->key;
      trec->bptr[0].d_ptr=NULL;
      trec->bptr[1].d_ptr=drec;
      drec->parent=trec;
      trec->c=0;
      for(i=1;i<tmax;i++)
      { trec->key[i].key_ptr= NULL; }
      for(i=2;i<tmax;i++)
      { trec->bptr[i].d_ptr=NULL; }
   }
 }
 else if(root==NULL)
 { int i=dmax-1; int done=0,count=0;
   while(i>0)
   { list_ptr->rec_ptr[i].ptr=list_ptr->rec_ptr[i-1].ptr;
     i--;
   }

   i=1;
   while(!done)
   { if(list_ptr->rec_ptr[i].ptr!=NULL)
     { if(equals(list_ptr->rec_ptr[i].ptr->key,rec->key)<0)
       { list_ptr->rec_ptr[i-1].ptr=list_ptr->rec_ptr[i].ptr;
         i++;
       }
       else
       { list_ptr->rec_ptr[i-1].ptr= rec; done=1; }
     }
     else
     { list_ptr->rec_ptr[i-1].ptr= rec; done=1; }
   }
   for(i=0;list_ptr->rec_ptr[i].ptr!=NULL;i++)
   {count++; }
   if (count>=dmin-1)//count
   { tree_node *trec;
     trec=(tree_node*)malloc(sizeof(tree_node));
     trec->parent=NULL;
     root=trec;
     trec->key[0].key_ptr=list_ptr->rec_ptr[0].ptr->key;
     trec->bptr[0].d_ptr=NULL;
     trec->bptr[1].d_ptr=list_ptr;list_ptr->parent=trec;
     trec->c=0;
     for(i=1;i<tmax;i++)
     { trec->key[i].key_ptr= NULL; }
     for(i=2;i<=tmax;i++)
     { trec->bptr[i].d_ptr=NULL; }
   }
   if(!done)
   { printf("error"); }
 }
 else
 { data_node *drec; int i=0,done=0;
   drec=insearch(&root,rec->key);//root is sent by address
   int d= present(drec,rec->key);
   if(d==1)
   { printf("already exists"); }
   else
    {while(i<dmax && !done)
     { if(drec->rec_ptr[i].ptr!=NULL)
       {/*printf("not null for i=%d",i);*/ if(equals(drec->rec_ptr[i].ptr->key,rec->key)<0)
         { i++;}
         else if(equals(drec->rec_ptr[i].ptr->key,rec->key)>0)
         { int j;//printf("ok im here");
            for( j=dmax-1;j>i;j--)
           { drec->rec_ptr[j].ptr= drec->rec_ptr[j-1].ptr; }


                  // printf("in case=>%d\n",root->key[0].key_ptr->roll);
           drec->rec_ptr[i].ptr=rec; done=1;
         }
       }
       else
       { drec->rec_ptr[i].ptr=rec;//printf("yess");
           done=1;// printf(" i====%d",i);
       }
     }
     if(!done)
     { printf("error"); }
    }

 }
return (root);
}




int count1(data_node *dnode)
{
    if(dnode!=NULL)
    {
       data_node *dp=dnode;
      int i=0,done=0,count=0;
      for(i=0;i<dmax && !done;i++)
     {
      if(dp->rec_ptr[i].ptr!=NULL)
      {
        count++;
      }
      else{done=1;}
     }
      return(count);
    }
    else{return(0);}

}

int count(tree_node *tnode)
{ //printf("tree node count");
  if(tnode!=NULL)
  {
    tree_node *tp=tnode;
    int i=0,done=0,count=0;
    for(i=0;i<tmax && done!=1;i++)
    {
      if(tp->key[i].key_ptr!=NULL)
      {
        count++;
      }
      else{done=1;}
    }
    return(count);
  }
  else{return(0);}
}




tree_node * delete1(data_node *dataN,int stu_k,rec_node *stu_rec,keyy *d_parent,tree_node *root)
{    int count,i;rec_node *temp;tree_node *Nparent=dataN->parent;
     int j=which_ptr(Nparent,dataN->rec_ptr[0].ptr->key);
     count=count1(dataN)-1;//printf("%d\n",Nparent->key[0].key_ptr->roll);
    if(stu_k==0 && Nparent->bptr[0].d_ptr!=dataN)
    {
        temp=stu_rec;
        for(i=0;i<count;i++)
        {
           dataN->rec_ptr[i].ptr=dataN->rec_ptr[i+1].ptr;
        }
        dataN->rec_ptr[count].ptr=NULL;
        d_parent=dataN->rec_ptr[0].ptr->key;free(temp);
    }
    else if(stu_k==0  && Nparent->bptr[0].d_ptr==dataN)
    {
       temp=stu_rec;
        for(i=0;i<count;i++)
        {
           dataN->rec_ptr[i].ptr=dataN->rec_ptr[i+1].ptr;
        }
        dataN->rec_ptr[count].ptr=NULL;free(temp);
    }
    else
    {
        temp=stu_rec;
        for(i=stu_k;i<count;i++)
        {
           dataN->rec_ptr[i].ptr=dataN->rec_ptr[i+1].ptr;
        }
        dataN->rec_ptr[count].ptr=NULL;
        free(temp);
    }
    if(count==0 && dataN==list_ptr)
    {
        list_ptr=dataN->next;  list_ptr->prev=NULL;
    }
    else if(count==0)
    {
        Nparent->bptr[j].d_ptr=NULL;
        dataN->prev->next=dataN->next;
        dataN->next->prev=dataN->prev;
        free(dataN);
    }
    printf("delete successful");
 return(root);
}

tree_node * delete2(data_node *dataN,data_node *l_dataN,int stu_k,rec_node *stu_rec,keyy *d_parent,tree_node *root)
{
   free(stu_rec);
   dataN->rec_ptr[stu_k].ptr=NULL;
   int count_l,i;
   count_l=count1(l_dataN)-1;
   for(i=stu_k;i>0;i--)
   {
       dataN->rec_ptr[i].ptr=dataN->rec_ptr[i-1].ptr;
   }
   dataN->rec_ptr[0].ptr=l_dataN->rec_ptr[count_l].ptr;
   l_dataN->rec_ptr[count_l].ptr=NULL;
   d_parent=dataN->rec_ptr[0].ptr->key;
   printf("delete successful");
return(root);
}

tree_node * delete3(data_node *dataN,data_node *r_dataN,int stu_k,rec_node *stu_rec,keyy *d_parent,tree_node *root)
{
   int count_r,i,count;tree_node *Nparent=dataN->parent;
   int j=which_ptr(Nparent,dataN->rec_ptr[0].ptr->key);
   count_r=count1(r_dataN)-1;
   count=count1(dataN);
   if(count-1==stu_k)
   {
      free(stu_rec);
      dataN->rec_ptr[stu_k].ptr=NULL;
      dataN->rec_ptr[stu_k].ptr=r_dataN->rec_ptr[0].ptr;
   }
   else
   {
       free(stu_rec);
       dataN->rec_ptr[stu_k].ptr=NULL;
       for(i=stu_k;i<count-1;i++)
       {
           dataN->rec_ptr[i].ptr=dataN->rec_ptr[i+1].ptr;
       }
       dataN->rec_ptr[count-1].ptr=r_dataN->rec_ptr[0].ptr;
   }
   for(i=0;i<count_r;i++)
   {
     r_dataN->rec_ptr[i].ptr=r_dataN->rec_ptr[i+1].ptr;
   }
   r_dataN->rec_ptr[count_r].ptr=NULL;
   d_parent=r_dataN->rec_ptr[0].ptr->key;
   if(count==0 && dataN==list_ptr)
    {
        list_ptr=dataN->next;  list_ptr->prev=NULL;
    }
    else if(count==0)
    {
        Nparent->bptr[j].d_ptr=NULL;
        dataN->prev->next=dataN->next;
        dataN->next->prev=dataN->prev;
        free(dataN);
    }
   printf("delete successful");
return(root);
}

tree_node *delete_reccur(tree_node *root,tree_node *sb_l,tree_node * Psb_l,keyy *Gparent)//c=1
{ int j= which_ptr(Psb_l->parent,Psb_l->key[0].key_ptr),l_count,found=0,gold,silver,k,i;tree_node *lp; keyy *temp; tree_node*ttemp;
  if(j!=0)//left pointer exists
  { lp=Psb_l->parent->bptr[j-1].t_ptr;
    l_count=count(lp);
    temp= Psb_l->parent->key[j-1].key_ptr;
    if( l_count-1<tmin)//WHEN LP COUNT IS LESS THAN MIN
    {
       while(found==0)
      {
         if(Psb_l->key[k].key_ptr==Gparent)
         {
             found=1;gold=k;silver=k+1;

         }k++;
      }
      for(i=gold;i<tmax-1;i++)
      {
         Psb_l->key[i].key_ptr=Psb_l->key[i+1].key_ptr;
      }Psb_l->key[tmax-1].key_ptr=NULL;
      for(j=silver;j<tmax;i++)//once please check this function ....i think mistake with p_count use
      {
        Psb_l->bptr[j].t_ptr=Psb_l->bptr[j+1].t_ptr;
      }Psb_l->bptr[tmax].t_ptr=NULL;
       lp->key[l_count].key_ptr=temp;
          for(i=l_count+1,j=0;j<tmin;i++,j++)
          { lp->key[i].key_ptr= Psb_l->key[j].key_ptr;
            Psb_l->key[j].key_ptr=NULL;
          }
          for(i=l_count+1,j=0;j<=tmin;i++,j++)
          { lp->bptr[i].t_ptr= Psb_l->bptr[j].t_ptr;
            Psb_l->bptr[j].t_ptr=NULL;
          }
          for(i=0;i<tmax;i++)
          { lp->bptr[i].t_ptr->parent=lp;}
          if( lp->parent==root)//case in lp count <min
          { int j= count(lp->parent);
            if(j==1)
            { ttemp=lp->parent;
              root= lp;
              lp->parent=NULL;
              free(ttemp);
              return (root);
            }
            else
            { found=0;
               while(found==0)
              {
                if(lp->parent->key[k].key_ptr==temp)
                {
                  found=1;gold=k;silver=k+1;
                }k++;
              }
              for(i=gold;i<tmax-1;i++)
              {
                 lp->parent->key[i].key_ptr=lp->parent->key[i+1].key_ptr;
              }lp->parent->key[tmax-1].key_ptr=NULL;
              for(j=silver;j<tmax;i++)//once please check this function ....i think mistake with p_count use
              {
                lp->parent->bptr[j].t_ptr=lp->parent->bptr[j+1].t_ptr;
              }lp->parent->bptr[tmax].t_ptr=NULL;
              return(root);
            }
          }
          else//one of the case in if lp count is < min
          { if(count(lp->parent)-1>=tmin)
            { found=0;
               while(found==0)
              {
                if(lp->parent->key[k].key_ptr==temp)
                {
                  found=1;gold=k;silver=k+1;
                }k++;
              }
              for(i=gold;i<tmax-1;i++)
              {
                 lp->parent->key[i].key_ptr=lp->parent->key[i+1].key_ptr;
              }lp->parent->key[tmax-1].key_ptr=NULL;
              for(j=silver;j<tmax;i++)//once please check this function ....i think mistake with p_count use
              {
                lp->parent->bptr[j].t_ptr=lp->parent->bptr[j+1].t_ptr;
              }lp->parent->bptr[tmax].t_ptr=NULL;
              return(root);
           }
           else
           { root= delete_reccur(root,lp,lp->parent,temp);
              return(root);
           }
         }
       }
       else//borrow from left that is lp count >min
       { for(i=tmax-1;i>0;i--)
         {
          Psb_l->key[i].key_ptr=Psb_l->key[i-1].key_ptr;
         }Psb_l->key[0].key_ptr=NULL;
         for(i=tmax;i>0;i--)
         {
          Psb_l->bptr[i].t_ptr=Psb_l->bptr[i-1].t_ptr;
         }Psb_l->bptr[0].t_ptr=NULL;
         Psb_l->key[0].key_ptr=temp;//bring temp down
         temp=lp->key[l_count].key_ptr;  lp->key[l_count].key_ptr=NULL;//key change
         Psb_l->bptr[0].t_ptr=lp->bptr[l_count].t_ptr;  lp->bptr[l_count].t_ptr=NULL;//child change
         Psb_l->bptr[0].t_ptr->parent=Psb_l;//parent change
         return(root);
       }
   }
   else//when there is no left then it is to be taken from right
   {
     lp=Psb_l->parent->bptr[j+1].t_ptr;
     l_count=count(lp);
    temp= Psb_l->parent->key[j].key_ptr;
    if( l_count-1<tmin)//WHEN RP COUNT IS LESS THAN MIN
    {
       while(found==0)
      {
         if(Psb_l->key[k].key_ptr==Gparent)
         {
             found=1;gold=k;silver=k+1;

         }k++;
      }
       for(i=gold;i<tmax-1;i++)
      {
         Psb_l->key[i].key_ptr=Psb_l->key[i+1].key_ptr;
      }Psb_l->key[tmax-1].key_ptr=NULL;
      for(j=silver;j<tmax;i++)//once please check this function ....i think mistake with p_count use
      {
        Psb_l->bptr[j].t_ptr=Psb_l->bptr[j+1].t_ptr;
      }Psb_l->bptr[tmax].t_ptr=NULL;
       Psb_l->key[tmin-1].key_ptr=temp;
          for(i=tmin,j=0;j<l_count;i++,j++)
          { Psb_l->key[i].key_ptr= lp->key[j].key_ptr;
            lp->key[j].key_ptr=NULL;//the rp ko null bhi karthy ja rahi hu
          }
          for(i=tmin+1,j=0;j<=l_count;i++,j++)
          { Psb_l->bptr[i].t_ptr=lp->bptr[j].t_ptr;
            lp->bptr[j].t_ptr=NULL;//the rp ka bptr bhi NULL kar rahi;
          }
          for(i=0;i<tmax;i++)
          { Psb_l->bptr[i].t_ptr->parent=Psb_l;}
          if( lp->parent==root)//case in lp count <min
          { int j= count(Psb_l->parent);
            if(j==1)
            { ttemp=Psb_l->parent;
              root=Psb_l;
              Psb_l->parent=NULL;
              free(ttemp);
              return (root);
            }
            else
            { found=0;
               while(found==0)
              {
                if(Psb_l->parent->key[k].key_ptr==temp)
                {
                  found=1;gold=k;silver=k+1;
                }k++;
              }
              for(i=gold;i<tmax-1;i++)
              {
                 Psb_l->parent->key[i].key_ptr=Psb_l->parent->key[i+1].key_ptr;
              }Psb_l->parent->key[tmax-1].key_ptr=NULL;
              for(j=silver;j<tmax;i++)//once please check this function ....i think mistake with p_count use
              {
                Psb_l->parent->bptr[j].t_ptr=Psb_l->parent->bptr[j+1].t_ptr;
              }Psb_l->parent->bptr[tmax].t_ptr=NULL;
              return(root);
            }
          }
          else//one of the case in if lp count is < min
          { if(count(lp->parent)-1>=tmin)
            { found=0;
               while(found==0)
              {
                if(Psb_l->parent->key[k].key_ptr==temp)
                {
                  found=1;gold=k;silver=k+1;
                }k++;
              }
              for(i=gold;i<tmax-1;i++)
              {
                 Psb_l->parent->key[i].key_ptr=Psb_l->parent->key[i+1].key_ptr;
              }Psb_l->parent->key[tmax-1].key_ptr=NULL;
              for(j=silver;j<tmax;i++)//once please check this function ....i think mistake with p_count use
              {
                Psb_l->parent->bptr[j].t_ptr=Psb_l->parent->bptr[j+1].t_ptr;
              }Psb_l->parent->bptr[tmax].t_ptr=NULL;
              return(root);
           }
           else
           { root= delete_reccur(root,Psb_l,Psb_l->parent,temp);
              return(root);
           }
         }
       }
       else//borrow from left that is lp count >min
       {
         Psb_l->key[tmin-1].key_ptr=temp;//temp is bought down
         temp=lp->key[0].key_ptr;//temp is changed with right starting
         Psb_l->bptr[tmin+1].t_ptr=lp->bptr[0].t_ptr;  Psb_l->bptr[tmin+1].t_ptr->parent=Psb_l;//parent changed
         for(i=0;i<tmax-1;i++)
         { lp->key[i].key_ptr=lp->key[i+1].key_ptr;
         } lp->key[tmax-1].key_ptr=NULL;
         for(i=0;i<tmax;i++)
         {
          lp->bptr[i].t_ptr=lp->bptr[i-1].t_ptr;
         }lp->bptr[tmax].t_ptr=NULL;

         return(root);
       }
  }
}


tree_node * delete4(data_node *dataN,data_node *r_dataN,data_node *l_dataN,tree_node *sb_l,tree_node *sb_r,int stu_k,rec_node *stu_rec,keyy *d_parent,keyy *Gparent,tree_node *root)
{
    int i=0,j=0,k=0,count_l,count12,count_r,p_count,found=0,gold,silver,pl_count;
    tree_node *bhai;
    count12=count1(dataN);
    count_l=count1(l_dataN);count_r=count1(r_dataN);p_count=count(dataN->parent);bhai=dataN->parent;
    pl_count=count(sb_l);
    if(l_dataN->parent==dataN->parent && r_dataN->parent==dataN->parent  )
    { //printf("first if\n");
        free(stu_rec);
        dataN->rec_ptr[stu_k].ptr=NULL;
       for(i=stu_k;i<dmax-1;i++)
       {
         dataN->rec_ptr[i].ptr=dataN->rec_ptr[i+1].ptr;
       }dataN->rec_ptr[dmax-1].ptr=NULL;i=0;
      while(j!=count12-1)
      {
          if(dataN->rec_ptr[i].ptr!=NULL)
          {//printf("first while");
            l_dataN->rec_ptr[count_l+i].ptr=dataN->rec_ptr[i].ptr;j++;
          }
          i++;
      }
      while(found==0)
      {
         if(bhai->key[k].key_ptr==d_parent)
         { printf("haa found\n");
             found=1;gold=k;silver=k+1;
         }k++;
      }
      if(p_count-1>=tmin)
      {
        for(i=gold;i<tmax-1;i++)
        {// printf("aww\n");
         bhai->key[i].key_ptr=bhai->key[i+1].key_ptr;
        }bhai->key[tmax-1].key_ptr=NULL;
        for(j=silver;j<tmax;j++)//once please check this function ....i think mistake with p_count use
        { //printf("eww\n");
        bhai->bptr[j].d_ptr=bhai->bptr[j+1].d_ptr;
        }bhai->bptr[tmax].d_ptr=NULL;
        if(dataN==list_ptr )
        {
            list_ptr=list_ptr->next; list_ptr->prev=NULL;
        }
        else
        {
            dataN->prev->next=dataN->next;
            dataN->next->prev=dataN->prev;
        }


      }
      else
      { //printf("offo!\n");
        if(pl_count-1>=tmin)
        {
          for(i=silver;i>0;i--)
          {
              bhai->bptr[i].d_ptr=bhai->bptr[i-1].d_ptr;
          }bhai->bptr[0].d_ptr=NULL;
          for(j=gold;j>0;j--)
          {
              bhai->key[j].key_ptr=bhai->key[j-1].key_ptr;
          }
          bhai->key[0].key_ptr=NULL;
          bhai->key[0].key_ptr=Gparent;
          Gparent=sb_l->key[pl_count-1].key_ptr;
          sb_l->key[pl_count-1].key_ptr=NULL;
          bhai->bptr[0].d_ptr=sb_l->bptr[pl_count].d_ptr;
          sb_l->bptr[pl_count].d_ptr=NULL;
          bhai->bptr[0].d_ptr->parent=bhai;
        }
        else if(bhai!=root)
        { //printf("okthen?");
          for(i=gold;i<tmax-1;i++)
          {
            bhai->key[i].key_ptr=bhai->key[i+1].key_ptr;
          }bhai->key[tmax-1].key_ptr=NULL;
          for(j=silver;j<tmax;j++)//once please check this function ....i think mistake with p_count use
          {
            bhai->bptr[j].d_ptr=bhai->bptr[j+1].d_ptr;
          }bhai->bptr[tmax].d_ptr=NULL;

          sb_l->key[pl_count].key_ptr=Gparent;
          for(i=pl_count+1,j=0;j<p_count-1;i++,j++)
          { sb_l->key[i].key_ptr= bhai->key[j].key_ptr; //printf("shifting");
            bhai->key[j].key_ptr=NULL;
          }
          for(i=pl_count+1,j=0;j<p_count;i++,j++)
          { sb_l->bptr[i].d_ptr= bhai->bptr[j].d_ptr;
            bhai->bptr[j].d_ptr=NULL;
          }
          for(i=0;i<tmax && sb_l->bptr[i].d_ptr!=NULL ;i++)
          { sb_l->bptr[i].d_ptr->parent=sb_l;}
          if(dataN==list_ptr){list_ptr=list_ptr->next;list_ptr->prev=NULL;}
          else
          {
              dataN->prev->next=dataN->next;
              dataN->next->prev=dataN->prev;
          }
          if(sb_l->parent==root && count(sb_l->parent)==1)//root gets chaged :)
          { printf("changing root\n");tree_node *ttemp=root;
            root=sb_l;
            free(ttemp);
          }
          else if(count(sb_l->parent)-1>=tmin || sb_l->parent==root)//c=1
          { found=0;
            while(found==0)
            {
              if(sb_l->parent->key[k].key_ptr==Gparent)
              {
               found=1;gold=k;silver=k+1;

              } k++;
            }
            for(i=gold;i<tmax-1;i++)
            {
              sb_l->parent->key[i].key_ptr=sb_l->parent->key[i+1].key_ptr;
            }sb_l->parent->key[tmax-1].key_ptr=NULL;
            for(j=silver;j<tmax;i++)//once please check this function ....i think mistake with p_count use
            {
             sb_l->parent->bptr[j].t_ptr=sb_l->parent->bptr[j+1].t_ptr;
            } sb_l->parent->bptr[tmax].t_ptr=NULL;
          }
          else//recursively call another function
          {// printf("oh no!\n");
               root=delete_reccur(root,sb_l,sb_l->parent,Gparent); }

      }

      printf("delete successful");
    }
   
  return(root);
    }
return(root);
}

tree_node *Search(keyy *key,tree_node *root)
{ //printf("u r in search\n");
    tree_node *RR=root,*sb_l=NULL,*sb_r=NULL;
    keyy *parent=NULL, *d_parent=NULL;
    data_node *dataN=NULL,*l_dataN=NULL, *r_dataN=NULL;
    rec_node *stu_rec=NULL;
    int i=0,j=0,k=0,l=0,found=0,stu_k=-1;
    int countRR=0,countSBL=0,countSBR=0,Dcount=0,DLcount=0,DRcount=0;
    countRR=count(RR); //count of the tree_node of the data_node which could contain the key
    while(RR->c==1)
    {
        found=0;
        i=0;
        while(found==0)
        {
            if((RR->key[i].key_ptr->roll)>(key->roll) && RR->c==1)
            {
                if(i!=0)
                {
                    parent=RR->key[i-1].key_ptr;
                    sb_l=RR->bptr[i-1].t_ptr;
                    sb_r=RR->bptr[i+1].t_ptr;
                    RR=RR->bptr[i].t_ptr;
                    found=1;
                }
                else
                {
                    parent=RR->key[0].key_ptr;
                    sb_r=RR->bptr[1].t_ptr;
                    RR=RR->bptr[0].t_ptr;
                    found=1;
                }
            }
            else if(j==countRR-1)//check for null condition wala case
            { //printf("last wala");
                parent=RR->key[j].key_ptr;
                sb_l=RR->bptr[j].t_ptr;
                //sb_r=RR->bptr[j+1].t_ptr;
                RR=RR->bptr[j+1].t_ptr;
                found=1;
            }
            else
            {
                i++;j++;
            }
        }
   }
    found=0;
    countRR=count(RR); //count of the tree_node of the data_node which could contain the key
    if(sb_l!=NULL){countSBL=count(sb_l);} //count of the left sibling tree_node
    if(sb_r!=NULL){countSBR=count(sb_r);} //count of the right sibling tree_node
    while(found==0)
    {
        if((RR->key[j].key_ptr->roll)==(key->roll) )
        {
            dataN=RR->bptr[j+1].d_ptr;
            l_dataN=RR->bptr[j].d_ptr;
            r_dataN=RR->bptr[j+2].d_ptr;
            d_parent=RR->key[j].key_ptr;
            found=1;
        }
        else if(((RR->key[j].key_ptr->roll)>(key->roll)))
        {
            dataN=RR->bptr[j].d_ptr;
            if(j!=0){l_dataN=RR->bptr[j-1].d_ptr;}
            else
            {
                if(sb_l!=NULL){l_dataN=sb_l->bptr[countSBL].d_ptr;}
            }
            r_dataN=RR->bptr[j+1].d_ptr;
            if(j!=0){d_parent=RR->key[j-1].key_ptr;}
            else{d_parent=RR->key[0].key_ptr;}
            found=1;
        }
        else if(j==countRR-1)
        {// printf("i am here\n");
           d_parent=RR->key[j].key_ptr;
           if( RR->bptr[j].d_ptr!=NULL){l_dataN=RR->bptr[j].d_ptr;}
           dataN=RR->bptr[j+1].d_ptr;
           if(sb_r!=NULL){r_dataN=sb_r->bptr[0].d_ptr;}
           found=1;
        }
        else{j++;}
    }
   Dcount=count1(dataN);
   if(l_dataN!=NULL){DLcount=count1(l_dataN);}
   if(r_dataN!=NULL){DRcount=count1(r_dataN);}
   found=0;
   while(found==0 && k<dmax)
   {//printf("to find stu_rec %d\n",dataN->rec_ptr[k].ptr->key->roll);
       if(dataN->rec_ptr[k].ptr->key->roll==key->roll && strcmp(dataN->rec_ptr[k].ptr->key->sub_code,key->sub_code)==0)
       {
           stu_rec=dataN->rec_ptr[k].ptr;
           stu_k=k;
           found=1;
       }
       else{k++;}
   }
   if(found==1)
   {
      printf("the key is found\n");
      if(Dcount-1<dmin && l_dataN==NULL && r_dataN==NULL)
      {
         // printf("initial case\n");
          root=delete1(dataN,stu_k,stu_rec,d_parent,root);
      }
      else if(Dcount-1>=dmin)//the data_node containing the key has records more than tmin then we just delete and adjust the records
      {
        printf("u r in delete 1\n");
        root=delete1(dataN,stu_k,stu_rec,d_parent,root);
      }
      else if(Dcount-1<=dmin && DLcount-1>=dmin && l_dataN!=NULL && l_dataN->parent==dataN->parent)
      { printf("u r in delete 2\n");
         root=delete2(dataN,l_dataN,stu_k,stu_rec,d_parent,root);
      }
      else if(Dcount-1<=dmin && (l_dataN==NULL || l_dataN->parent!= dataN->parent) && DRcount-1>=dmin)
      { printf("u r in delete 3\n");
         root=delete3(dataN,r_dataN,stu_k,stu_rec,d_parent,root);
      }
      else if(Dcount-1<dmin&& DLcount-1<dmin && DRcount-1>=dmin && r_dataN->parent==dataN->parent)
      { printf("u r in delete 3\n");
          root=delete3(dataN,r_dataN,stu_k,stu_rec,d_parent,root);
      }
      else if(Dcount-1<=dmin && DLcount-1<=dmin && DRcount-1<=dmin)
      { printf("u r in delete 4\n");
          root=delete4(dataN,r_dataN,l_dataN,sb_l,sb_r,stu_k,stu_rec,d_parent,parent,root);
      }
      
   }
   else
   {
       printf("\nthe given key is not present in the records ");
   }
 return(root);
}


void failures( tree_node *root, char *sub, int pass)
{ data_node *temp=list_ptr; int i,r=0; //printf("hii");
  while(temp!=NULL)
  { for(i=0;i<dmax && temp->rec_ptr[i].ptr!=NULL &&temp->rec_ptr[i].ptr->key->roll!=r;i++)
    { if(strcmp(temp->rec_ptr[i].ptr->key->sub_code,sub)==0)
      {  r=temp->rec_ptr[i].ptr->key->roll;
         if(temp->rec_ptr[i].ptr->marks<pass)
         { printf("%d %s", temp->rec_ptr[i].ptr->key->roll,temp->rec_ptr[i].ptr->key->sub_code);}
      }
    }
    temp=temp->next;
  }
}

void kthHighest(tree_node *root, char *sub, int k)
{ data_node *temp=list_ptr; int max=5000000,max1=0,i,r=0,p=k; rec_node *rec,*rec2;
 while(k>0)
{ temp=list_ptr;max1=0;r=0;
  while(temp!=NULL)
  { for(i=0;i<dmax && temp->rec_ptr[i].ptr!=NULL &&temp->rec_ptr[i].ptr->key->roll!=r;i++)
    { if(strcmp(temp->rec_ptr[i].ptr->key->sub_code,sub)==0)
      { r=temp->rec_ptr[i].ptr->key->roll;
        if(temp->rec_ptr[i].ptr->marks>max1 && temp->rec_ptr[i].ptr->marks<max)
        { max1=temp->rec_ptr[i].ptr->marks;
          rec=temp->rec_ptr[i].ptr;
        }
      }
    }
    temp=temp->next;
  }
  max=max1;
  rec2=rec;//printf("\n%dth highest in %s sunject is %d %s",p,sub,rec2->key->roll,rec2->key->sub_code);
  k--;
}
printf("\n%dth highest in %s sunject is %d %s",p,sub,rec2->key->roll,rec2->key->sub_code);
}

void getTopper(tree_node *root)
{






}
void set_root(tree_node **root)
{ tree_node *rr=*root;int i=0,k=0,j;
if(rr->c==1)
{
 while(rr->c==1 && rr->bptr[i].t_ptr!=NULL)
 { rr=rr->bptr[i].t_ptr;
   if(rr->c==1 && rr->bptr[k].t_ptr!=NULL)
   {
     rr=rr->bptr[k].t_ptr;
   }
   else if(rr->c==1 && rr->bptr[k].t_ptr==NULL)
   { k=0;i++; }
   else if(rr->c==0)
   { for(  j=0;j<tmax && rr->key[j].key_ptr!=NULL;j++)
    { rr->key[j].key_ptr=rr->bptr[j+1].d_ptr->rec_ptr[0].ptr->key;}
    i++; rr=*root;
   }

 }

}
else
{
 for(  j=0;j<tmax && rr->key[j].key_ptr!=NULL;j++)
    { rr->key[j].key_ptr=rr->bptr[j+1].d_ptr->rec_ptr[0].ptr->key;}
}
}

void level_traversal(tree_node *root,int i)
{ tree_node *rr=root;
  if(rr->c==1)
  { while(rr->bptr[i].t_ptr!=NULL)
    { level_traversal(rr->bptr[i].t_ptr,i);
      i++;
    }
    printf("\n");
    for(int j=0;j<tmax && rr->key[j].key_ptr!=NULL;j++)
    { printf("%d%s\t",rr->key[j].key_ptr->roll,rr->key[j].key_ptr->sub_code); }
   
  }
  
  else
  { for(int j=0;j<tmax && rr->key[j].key_ptr!=NULL;j++)
    { printf("%d%s\t",rr->key[j].key_ptr->roll,rr->key[j].key_ptr->sub_code); }
    //printf("\n");
  }
}

void range_search(tree_node *root,keyy *key1, keyy *key2)
{ tree_node *trec=root, *trec1; int done=0,i,res,j; data_node *drec, *drec2; rec_node *stu_temp;
  while( trec->c==1)
  { done=0;
    for(i=0;i<tmax && !done;i++)
    { if(trec->key[i].key_ptr!=NULL)
       { res=equals(trec->key[i].key_ptr,key1);
         if( res>0)
         { trec=trec->bptr[i].t_ptr;
           done=1;
         }
         else if(res==0)
         { trec=trec->bptr[i+1].t_ptr;
           done=1;
         }
       }
       else
       { trec=trec->bptr[i].t_ptr; done=1;}
    }
    if(!done)
    { trec=trec->bptr[tmax].t_ptr; }
  }
  done=0;
  for(i=0;i<tmax  && !done;i++)//here c=0
  { if(trec->key[i].key_ptr!=NULL)
    { if(equals(trec->key[i].key_ptr,key1)>0)
      { drec=trec->bptr[i].d_ptr; done=1;j=i;}
      else if(equals(trec->key[i].key_ptr,key1)==0)
      { drec=trec->bptr[i+1].d_ptr; done=1; j=i+1;}
    }
    else
    { drec=trec->bptr[i].d_ptr; done=1;j=i;}
  }
  //if(drec->rec_ptr[0].ptr!=NULL)
   //{printf(" spl if%d \n",drec->rec_ptr[0].ptr->key->roll);}
  if(!done)
  { j=tmax; drec=trec->bptr[j].d_ptr;}
  //from here go with list_ptr only
  done=0;
 for(j=0;j<dmax && drec->rec_ptr[j].ptr!=NULL && !done;j++)
 { if( equals(drec->rec_ptr[j].ptr->key,key1)>=0)
   { done=1; }
 } done=0;
 for(j=j-1;j<dmax && drec->rec_ptr[j].ptr!=NULL && !done;j++)
 { if( equals(drec->rec_ptr[j].ptr->key,key2)>0)
   { done=1; }
   else
   { printf("%d%s \t",drec->rec_ptr[j].ptr->key->roll,drec->rec_ptr[j].ptr->key->sub_code); }
 }
 drec=drec->next;
 while(!done && drec!=NULL)
 { //printf("final one");
  for(j=0;j<dmax && drec->rec_ptr[j].ptr!=NULL && !done;j++)
  { if( equals(drec->rec_ptr[j].ptr->key,key2)>0)
    { done=1; }
    else
    { printf("%d%s \t",drec->rec_ptr[j].ptr->key->roll,drec->rec_ptr[j].ptr->key->sub_code); }
  }
  drec=drec->next;
 }
}


int main()
{ int i=0;
keyy *key1,*key2;
rec_node *rec1; char c[10];
printf("%d %d",dmax,tmax);
tree_node *root=NULL;
while(i<5)
{
 key1=(keyy*)malloc(sizeof(keyy));
printf("give roll no. and subject code and marks");
scanf("%d",&key1->roll);
scanf("%s",key1->sub_code);
rec1=(rec_node*)malloc(sizeof(rec_node));
scanf("%d",&rec1->marks);
//printf("give marks,sem no,name,dept");
/*
scanf("%d",&rec1->sem);
scanf("%s",rec1->name);
scanf("%s",rec1->dept);*/
rec1->key=key1;
root=insert(root,rec1);
/*if(root!=NULL)
{printf("root_in main=%d\n",root->key[0].key_ptr->roll);}*/
 /*if(root->key[1].key_ptr!=NULL){printf("next=%d",root->key[1].key_ptr->roll); printf("%d",root->bptr[0].d_ptr->rec_ptr[0].ptr->key->roll);
 if(root->key[2].key_ptr!=NULL){printf("next=%d",root->key[2].key_ptr->roll);printf("%d",root->bptr[1].d_ptr->rec_ptr[0].ptr->key->roll);printf("%d",root->bptr[2].d_ptr->rec_ptr[0].ptr->key->roll);
if(root->key[3].key_ptr!=NULL){printf("next=%d",root->key[3].key_ptr->roll);printf("%d",root->bptr[3].d_ptr->rec_ptr[0].ptr->key->roll);
}}}*/
i++;
}
/*if(root==NULL)
{printf("null");}
else
{printf("'not null");}*/
print();
level_traversal(root,0);

printf("\n");

i=0;
do
{
printf("give a range to search");
key1=(keyy*)malloc(sizeof(keyy));
printf("give roll no. and subject code");
scanf("%d",&key1->roll);
scanf("%s",key1->sub_code);
key2=(keyy*)malloc(sizeof(keyy));
printf("give roll no. and subject code");
scanf("%d",&key2->roll);
scanf("%s",key2->sub_code);
range_search(root,key1,key2);
//printf("\n%d",root->bptr[0].d_ptr->rec_ptr[0].ptr->key->roll);
printf("do u want to continue?");
scanf("%d",&i);
}while(i==1);

do{
printf("\n what do u want to delete?");
key1=(keyy*)malloc(sizeof(keyy));
printf("give roll no. and subject code");
scanf("%d",&key1->roll);
scanf("%s",key1->sub_code);
root=Search(key1,root);
set_root(&root);

print();
level_traversal(root,0);
printf("do u want to continue?");
scanf("%d",&i);
}while(i==1);

printf("\ngive sub n pass marks to know failures");
scanf("%s",c);
scanf("%d",&i);
failures(root,c,i);

printf("\n give sub n value of k for kth highest");
scanf("%s",c);
scanf("%d",&i);
kthHighest(root,c,i);

/*
if(root->key[1].key_ptr!=NULL){printf("next=%d",root->key[1].key_ptr->roll); printf("%d",root->bptr[0].d_ptr->rec_ptr[0].ptr->key->roll);
 if(root->key[2].key_ptr!=NULL){printf("next=%d",root->key[2].key_ptr->roll);printf("%d",root->bptr[1].d_ptr->rec_ptr[0].ptr->key->roll);printf("%d",root->bptr[2].d_ptr->rec_ptr[0].ptr->key->roll);
if(root->key[3].key_ptr!=NULL){printf("next=%d",root->key[3].key_ptr->roll);printf("%d",root->bptr[3].d_ptr->rec_ptr[0].ptr->key->roll);
}}}
*/
return(0);
}
