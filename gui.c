#include<stdio.h>
#include<sqlite3.h>
#include<gtk/gtk.h>
#include "socket_client.h"
// Function declarations
void on_chat_clicked(GtkWidget *widget, gpointer data,char* ip);
void on_done_clicked(GtkWidget *widget, gpointer* data);
void connect_to_ip(GtkWidget* widget, gpointer* data);
void create_add_user_window(GtkWidget *widget, gpointer data);
void user_added(GtkWidget* widget,gpointer* data);
void quit__area(GtkWidget* widget,gpointer* data);
void show_con_list(GtkWidget* widget,gpointer* data);

int is_server = 1;
pthread_t server_thread;
int main(int argc, char *argv[])
{
    // Initialize GTK
    gtk_init(&argc, &argv);
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chat App");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    // Create container for content
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(window), box);
    
    // Create buttons
    GtkWidget *button_add_user = gtk_button_new_with_label("Add User");
    gtk_widget_set_size_request(button_add_user, -1, 50); // Set size request for button
    g_signal_connect(button_add_user, "clicked", G_CALLBACK(create_add_user_window), window);


    GtkWidget *button_chat = gtk_button_new_with_label("Chat");
    gtk_widget_set_size_request(button_chat, -1, 50); // Set size request for button
    g_signal_connect(button_chat, "clicked", G_CALLBACK(on_chat_clicked),window);


    GtkWidget *button_quit = gtk_button_new_with_label("Quit");
    gtk_widget_set_size_request(button_quit, -1, 50); // Set size request for button
    g_signal_connect(button_quit, "clicked", G_CALLBACK(gtk_main_quit),NULL);


    // Packing all the buttons in the vertical box
    gtk_box_pack_start(GTK_BOX(box), button_add_user, TRUE, TRUE, 3);
    gtk_box_pack_start(GTK_BOX(box), button_chat, TRUE, TRUE, 3);
    gtk_box_pack_start(GTK_BOX(box), button_quit, TRUE, TRUE, 3);
    // Show all widgets
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}

/* *Distinction between the main function and the other UI window methods.*/

// Callback function for Chat button click
void on_chat_clicked(GtkWidget *widget, gpointer data,char* ip)
{
    GtkWidget *main_window = GTK_WIDGET(data);
    // Create new chat window
    GtkWidget *chat_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(chat_window), "Chat");
    gtk_window_set_default_size(GTK_WINDOW(chat_window), 500, 500);
    gtk_window_set_position(GTK_WINDOW(chat_window), GTK_WIN_POS_CENTER);
    g_signal_connect(chat_window, "destroy", G_CALLBACK(gtk_main_quit), main_window);

    // Created container for content
    GtkWidget *chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,8);
    gtk_container_add(GTK_CONTAINER(chat_window), chat_box);

    // Created receiver name entry
    GtkWidget *receiver_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(receiver_entry), "Enter receiver name...");
    gtk_box_pack_start(GTK_BOX(chat_box), receiver_entry, FALSE, FALSE, 1);



    // Create message entry
    GtkWidget *message_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(message_entry), "Type your message...");
    // Creating the status bar
    GtkWidget* st_label = gtk_label_new("Not Connected");
    //Initialising text box
    GtkWidget* text_box = gtk_text_view_new();

    GtkWidget** dabba = (GtkWidget**)malloc(sizeof(GtkWidget*)*4);
    dabba[0] = receiver_entry;
    dabba[1] = st_label;
    dabba[2] = message_entry;
    dabba[3] = text_box;
    // Creating the contact List Button:
    GtkWidget** dabba2 = (GtkWidget**)malloc(sizeof(GtkWidget*)*2);
    dabba2[0] = chat_window;
    dabba2[1] = main_window;
    GtkWidget* con_list_button = gtk_button_new_with_label("Show Contacts");
    g_signal_connect(con_list_button,"clicked",G_CALLBACK(show_con_list),dabba2);
    gtk_box_pack_start(GTK_BOX(chat_box),con_list_button,FALSE,FALSE,1);

    // Created the connect button

    GtkWidget* conn_button = gtk_button_new_with_label("Connect to User");
    g_signal_connect(conn_button,"clicked",G_CALLBACK(connect_to_ip),dabba);
    gtk_box_pack_start(GTK_BOX(chat_box), conn_button, FALSE, FALSE, 1);
    pthread_join(server_thread,NULL);


    // The status bar is to be shown below the connect button, but needs to be initialised before the connect button so as to update the status of connection on button click for which we need to pass it as data in the Callback function.
    gtk_box_pack_start(GTK_BOX(chat_box),st_label,FALSE,FALSE,1);

    // Created the text box to display current chats in a scrolled window
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(chat_box), scrolled_window);
    gtk_widget_set_size_request(scrolled_window, 1, 150);

    // Created the text_box to display chats.
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_box), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_box);
    
    gtk_box_pack_start(GTK_BOX(chat_box), message_entry, FALSE, FALSE, 1);

    // Create the packer in order to pass multiple gtkWidgets into the callback function
    GtkWidget** pack = (GtkWidget**)malloc(sizeof(GtkWidget*)*2);
    pack[0] = message_entry;
    pack[1] = text_box;

    // Create "Done" button
    GtkWidget *button_done = gtk_button_new_with_label("Done");
    g_signal_connect(button_done, "clicked", G_CALLBACK(on_done_clicked),pack);
    gtk_box_pack_start(GTK_BOX(chat_box), button_done,FALSE, FALSE, 1);

    // Creating one another packing for passing multiple GtkWidgets as data into the callback function.
    GtkWidget** pack2 = (GtkWidget**)malloc(sizeof(GtkWidget*)*2);
    pack2[0] = main_window;
    pack2[1] = chat_window;

    // Quit Chat BUtton
    GtkWidget* quit_chat = gtk_button_new_with_label("Quit this Chat");
    gtk_box_pack_start(GTK_BOX(chat_box),quit_chat,FALSE,FALSE,1);
    g_signal_connect(quit_chat,"clicked",G_CALLBACK(quit__area),pack2);
    // Show all widgets
    gtk_widget_show_all(chat_window);
    gtk_widget_hide(main_window);
}


void show_con_list(GtkWidget* widget,gpointer* data)
{
    GtkWidget* cont_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(cont_window), "Contacts");
    gtk_window_set_default_size(GTK_WINDOW(cont_window), 500, 500);
    gtk_window_set_position(GTK_WINDOW(cont_window), GTK_WIN_POS_CENTER);
    g_signal_connect(cont_window, "destroy", G_CALLBACK(gtk_widget_destroy), data[1]);
    
    GtkWidget* master_over = gtk_box_new(GTK_ORIENTATION_VERTICAL,1);
    gtk_container_add(GTK_CONTAINER(cont_window),master_over);

    GtkWidget* mast_hor_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,1);
    gtk_box_pack_start(GTK_BOX(master_over),mast_hor_box,TRUE,TRUE,1);

    GtkWidget* left_ver_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,1);
    GtkWidget* right_ver_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,1);

    GtkWidget* left_label = gtk_label_new("Name");
    GtkWidget* right_label = gtk_label_new("IP");

    gtk_box_pack_start(GTK_BOX(mast_hor_box),left_ver_box,TRUE,TRUE,1);
    gtk_box_pack_start(GTK_BOX(mast_hor_box),right_ver_box,TRUE,TRUE,1);

    gtk_box_pack_start(GTK_BOX(left_ver_box),left_label,FALSE,FALSE,1);
    gtk_box_pack_start(GTK_BOX(right_ver_box),right_label,FALSE,FALSE,1);

    GtkWidget* quit_button = gtk_button_new_with_label("Back");
    GtkWidget** packer = (GtkWidget**)malloc(sizeof(GtkWidget*)*2);
    packer[0] = data[1];
    packer[1] = cont_window;

    // Created the text box to display current chats in a scrolled window_left
    GtkWidget *scrolled_window_left = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_left),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(left_ver_box), scrolled_window_left);
    GtkWidget* text_box_left = gtk_text_view_new();
    gtk_widget_set_size_request(scrolled_window_left, 0.5, 450);

    // Created the text box to display current chats in a scrolled window_right
    GtkWidget *scrolled_window_right = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_right),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(right_ver_box), scrolled_window_right);
    GtkWidget* text_box_right = gtk_text_view_new();
    gtk_widget_set_size_request(scrolled_window_right,0.5, 450);

    // Created the text_box to display chats.
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_box_left), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_window_left), text_box_left);
    // Created the text_box to display chats.
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_box_right), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_window_right), text_box_right);

    g_signal_connect(quit_button,"clicked",G_CALLBACK(quit__area),packer);
    gtk_box_pack_start(GTK_BOX(master_over),quit_button,FALSE,FALSE,1);


    // Adding the data to the text_boxes in ordered manner by reading from the CSV.
    GtkTextIter iter_left;
    GtkTextBuffer* buffer_left;
    buffer_left = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_box_left));
    gtk_text_buffer_get_end_iter(buffer_left,&iter_left);
    gtk_text_buffer_get_end_iter(buffer_left,&iter_left);

    GtkTextIter iter_right;
    GtkTextBuffer* buffer_right;
    buffer_right = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_box_right));
    gtk_text_buffer_get_end_iter(buffer_right,&iter_right);

    sqlite3* db;
    FILE* fp = fopen("./Contacts/Contacts.db","r");
    int rc = 1;
    if(fp == NULL)
    {
        system("mkdir Contacts");
        rc = sqlite3_open("./Contacts/Contacts.db",&db);
        char* query = "Create Table Contacts(\"CON_NAME TEXT NOT NULL PRIMARY KEY\",\"IP_ADDRESS TEXT NOT NULL\")";
        sqlite3_exec(db,query,NULL,NULL,NULL);
    }
    else
    {
        rc = sqlite3_open("./Contacts/Contacts.db",&db);
    }
    if(rc == SQLITE_OK)
    {
        printf("Databse opened successfully\n");
    }
    else
    {
        printf("Couldn't open database\n");
    }
    sqlite3_stmt* stmt = NULL;
    char* query = (char*)malloc(sizeof(char)*200);
    strcpy(query,"SELECT * FROM Contacts;");
    sqlite3_prepare_v2(db,query,-1,&stmt,0);
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        gtk_text_buffer_insert(buffer_left,&iter_left,(const gchar*)sqlite3_column_text(stmt,0),-1);
        gtk_text_buffer_get_end_iter(buffer_left,&iter_left);
        gtk_text_buffer_insert(buffer_left,&iter_left,"\n\n",-1);

        gtk_text_buffer_insert(buffer_right,&iter_right,(const gchar*)sqlite3_column_text(stmt,1),-1);
        gtk_text_buffer_get_end_iter(buffer_right,&iter_right);
        gtk_text_buffer_insert(buffer_right,&iter_right,"\n\n",-1);
    }

    gtk_widget_show_all(cont_window);
    gtk_widget_hide((GTK_WIDGET(data[0])));
}
// Created the function to quit the chat_window
void quit__area(GtkWidget* widget,gpointer* data)
{
    gtk_widget_show_all((GtkWidget*)data[0]);
    gtk_widget_hide((GtkWidget*)data[1]);
    return;
}

// Callback function for Done button click in chat window
void on_done_clicked(GtkWidget *widget, gpointer* data)
{
    GtkWidget *message_inp = GTK_WIDGET(data[0]);
    GtkWidget *text_box = GTK_WIDGET(data[1]);
    const gchar* message = gtk_entry_get_text((GtkEntry*)message_inp);
    GtkTextIter iter;
    GtkTextBuffer* buffer;
    struct packer* dummy_pack = (struct packer*)malloc(sizeof(struct packer));
    dummy_pack->data = data;
    dummy_pack->sockf = NULL;
    if(is_server)
    {
        send_m_server(dummy_pack);
    }
    else
    {
        send_m(dummy_pack);
    }
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_box));
    gtk_text_buffer_get_end_iter(buffer,&iter);
    gtk_text_buffer_insert(buffer,&iter,"You: ",-1);
    gtk_text_buffer_get_end_iter(buffer,&iter);
    gtk_text_buffer_insert(buffer,&iter,message,-1);
    gtk_text_buffer_get_end_iter(buffer,&iter);
    gtk_text_buffer_insert(buffer,&iter,"\n",-1);
    gtk_entry_set_text((GtkEntry*)message_inp,"");
}   

// Establishing connection with the user
void connect_to_ip(GtkWidget* widget,gpointer* data)
{
    GtkWidget* rec_name_panel = GTK_WIDGET(data[0]);
    const gchar* rec_name = gtk_entry_get_text((GtkEntry*)rec_name_panel);
    sqlite3* db;
    FILE* fp = fopen("./Contacts/Contacts.db","r");
    int rc = 1;
    if(fp == NULL)
    {
        system("mkdir Contacts");
        rc = sqlite3_open("./Contacts/Contacts.db",&db);
    }
    else
    {
        rc = sqlite3_open("./Contacts/Contacts.db",&db);
    }
    if(rc == SQLITE_OK)
    {
        printf("Databse opened successfully\n");
    }
    else
    {
        printf("Couldn't open database\n");
    }
    sqlite3_stmt* stmt = NULL;
    char* query = (char*)malloc(sizeof(char)*200);
    strcpy(query,"SELECT * FROM Contacts;");
    sqlite3_prepare_v2(db,query,-1,&stmt,0);
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        if(strcmp(rec_name,(char*)sqlite3_column_text(stmt,0)) == 0)
        {
            printf("%s\n",(char*)sqlite3_column_text(stmt,1));
            is_server = 0;
            int con_st = connect_ip((char*)sqlite3_column_text(stmt,1),data);
            if(con_st == 0)
            {
                int* connfd;
                char* client_ip;
                struct chat_wind_helper* helper = (struct chat_wind_helper*)malloc(sizeof(struct chat_wind_helper));
                helper->cl_ip = client_ip;
                helper->connfd = connfd;
                // begin_server((void*)helper);
                pthread_create(&server_thread,NULL,begin_server,(void*)helper);
            }
        }
    }
    GtkWidget* st_label = GTK_WIDGET(data[1]);
    gtk_label_set_label(GTK_LABEL(st_label),"Could not Connected");
}




/*For Distinction between the Chat UI and Add User UI*/



void create_add_user_window(GtkWidget *widget, gpointer data)
{
    GtkWidget *main_window = GTK_WIDGET(data);
    // Create the new window
    GtkWidget *add_user_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(add_user_window), "Add User");
    gtk_window_set_default_size(GTK_WINDOW(add_user_window), 500, 500);
    gtk_window_set_position(GTK_WINDOW(add_user_window), GTK_WIN_POS_CENTER);
    // Create a vertical box container to hold the sections
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_add(GTK_CONTAINER(add_user_window), box);
    

    // Create a label and entry for adding user's name
    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_box_pack_start(GTK_BOX(box), name_label, FALSE, FALSE, 1);

    GtkWidget *name_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), name_entry, FALSE, FALSE, 1);

    // Create a label and entry for adding user's IP address
    GtkWidget *ip_label = gtk_label_new("IP Address:");
    gtk_box_pack_start(GTK_BOX(box), ip_label, FALSE, FALSE, 1);

    GtkWidget *ip_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), ip_entry, FALSE, FALSE, 1);

    // Creating a pair of the name and ip label to pass into the user_added function
    GtkWidget** pair = (GtkWidget**)malloc(sizeof(GtkWidget*)*2);
    pair[0] = name_entry;
    pair[1] = ip_entry;
    // Create the done button:
    GtkWidget* done_button = gtk_button_new_with_label("Add Contact");
    g_signal_connect(done_button, "clicked", G_CALLBACK(user_added),pair);
    gtk_box_pack_start(GTK_BOX(box),done_button,FALSE,FALSE,1);

    GtkWidget** pair2 = (GtkWidget**)malloc(sizeof(GtkWidget*)*2);
    pair2[0] = main_window;
    pair2[1] = add_user_window;
    GtkWidget* quit_button = gtk_button_new_with_label("Go Back");
    g_signal_connect(quit_button,"clicked",G_CALLBACK(quit__area),pair2);
    gtk_box_pack_start(GTK_BOX(box),quit_button,FALSE,FALSE,1);
    // Show all widgets
    gtk_widget_show_all(add_user_window);
    gtk_widget_hide(main_window);
}

//Cretaed the function to update the contact list; 
void user_added(GtkWidget* widget,gpointer* data)
{
    GtkWidget* name_entry = GTK_WIDGET(data[0]);
    GtkWidget* ip_entry = GTK_WIDGET(data[1]);
    const gchar* name = gtk_entry_get_text(GTK_ENTRY(name_entry));
    const gchar* ip = gtk_entry_get_text(GTK_ENTRY(ip_entry));
    sqlite3* db;
    FILE* fp = fopen("./Contacts/Contacts.db","r");
    int rc;
    if(fp == NULL)
    {
        system("mkdir Contacts");
        rc = sqlite3_open("./Contacts/Contacts.db",&db);
        if(rc)
        {
            printf("Database not Created\n");
            return;
        }
        else
        {
            printf("Database Created Successfully\n");
        }
        char* query = "Create Table Contacts(\"CON_NAME TEXT NOT NULL PRIMARY KEY\",\"IP_ADDRESS TEXT NOT NULL\")";
        sqlite3_exec(db,query,NULL,NULL,NULL);
    }
    else
    {
        rc = sqlite3_open("./Contacts/Contacts.db",&db);
        if(rc)
        {
            printf("Unable to open database\n");
            return;
        }
        else
        {
            printf("Opened database successfully\n");
        }
    }
    char* query = (char*)calloc(1024,sizeof(char));
    strcpy(query,("INSERT INTO Contacts VALUES(\""));
    strcat(query,name);
    strcat(query,"\",\"");
    strcat(query,ip);
    strcat(query,"\")");
    rc = sqlite3_exec(db,query,NULL,NULL,NULL);
    free(query);
    sqlite3_free(db);
}