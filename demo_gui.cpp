#include <gtk/gtk.h>
#include <glib.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "GenTreeBillboardTexture.h"
#include <iostream>
#include <fstream>


/** Dialog widgets */
typedef struct _guidialog
{
    GtkWidget  *window;

    GtkWidget *gui_cancel;
    GtkWidget *gui_sdldemo;
    GtkWidget *gui_sdldemo_surface;
    GtkWidget *gui_generate;

    /* Share informations */
    GtkWidget *gui_width;
    GtkWidget *gui_height;
    GtkWidget *gui_seed;
    GtkWidget *gui_depth;

} GuiDialog;


/** Dialog window */
GuiDialog guidialog;



/*
void
gui_dialog_destroy ()
{
    gtk_widget_destroy (guidialog.gui_height);
    gtk_widget_destroy (guidialog.gui_width);
    gtk_widget_destroy (guidialog.gui_generate);
    gtk_widget_destroy (guidialog.gui_cancel);
    gtk_widget_destroy (guidialog.window);

    cout << "Destroy - gui_dialog_destroy ()" << endl;
}
*/

/**
* Generete button - call functions to generate tree texture
*/
void
generate_tree ( void )
{
     const gchar *width_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_width));
     const gchar *height_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_height));
     const gchar *seed_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_seed));
     const gchar *depth_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_depth));

    //Prevzato z demo.cpp
    const size_t width = atoi(width_str);
	const size_t height = atoi(height_str);
    const size_t myseed = atoi(seed_str);
	const size_t mydepth = atoi(depth_str);

	char asciiart[height][width];
	//char *asciiart = new char[int(height)];
	ofstream xpm;
	xpm.open("tree.xpm");
	xpm << "/* XPM */\n";
	xpm << "static char * tree[] = {\n";
	xpm << '"' << width << ' ' << height << " 2 1\",\n";
	xpm << "\"# c #000000\",\n";
	xpm << "\". c #ffffff\",\n";

	GenTreeBillboardTexture((char*)asciiart, width, height, myseed, mydepth);

	for (size_t y = 0; y < height; y++) {
		xpm << '"';
		for (size_t x = 0; x < width; x++) {
			xpm << asciiart[y][x];
		}
		xpm << "\",\n";
	}
	xpm << '}';
	xpm.close();


    cout << "generate_tree to tree.xpm - DONE" << endl;
}


void
run_sdldemo ( void )
{
    const gchar *width_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_width));
    const gchar *height_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_height));
    const gchar *seed_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_seed));
    const gchar *depth_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_depth));

    const gchar *gui_argv[7];
    GError *error;

    gui_argv[0] = "demo_sdl";
    gui_argv[1] = "xpm";
    gui_argv[2] = width_str;
    gui_argv[3] = height_str;
    gui_argv[4] = seed_str;
    gui_argv[5] = depth_str;
    gui_argv[6] = NULL;

    GSpawnFlags flag = G_SPAWN_CHILD_INHERITS_STDIN;
    error = NULL;
    g_spawn_async (NULL,        /* working_directory */
                    (gchar **) gui_argv,
                    NULL,        /* envp */
                    flag,           /* flags */
                    NULL,        /* child_setup */
                    NULL,        /* user_data */
                    NULL,        /* child_pid */
                    &error);

    if (error != NULL)
    {
            g_warning ("Error launching sfshare-gui: %s", error->message);
            g_error_free (error);
    }
}

void
run_sdldemo_surface ( void )
{
    const gchar *width_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_width));
    const gchar *height_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_height));
    const gchar *seed_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_seed));
    const gchar *depth_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_depth));

    const gchar *gui_argv[7];
    GError *error;

    gui_argv[0] = "demo_sdl";
    gui_argv[1] = "sdl";
    gui_argv[2] = width_str;
    gui_argv[3] = height_str;
    gui_argv[4] = seed_str;
    gui_argv[5] = depth_str;
    gui_argv[6] = NULL;

    GSpawnFlags flag = G_SPAWN_CHILD_INHERITS_STDIN;
    error = NULL;
    g_spawn_async (NULL,        /* working_directory */
                    (gchar **) gui_argv,
                    NULL,        /* envp */
                    flag,           /* flags */
                    NULL,        /* child_setup */
                    NULL,        /* user_data */
                    NULL,        /* child_pid */
                    &error);

    if (error != NULL)
    {
            g_warning ("Error launching sfshare-gui: %s", error->message);
            g_error_free (error);
    }
}

/**
* Create main window
*/
int
main ( int argc,  char **argv )
{
    GtkBuilder *builder;
    const gchar *ui_filename;
    GError *error = NULL;

    gtk_init( &argc, &argv );

    /* Create builder and load interface */
    builder = gtk_builder_new ();

    ui_filename = "demo_gui.ui";

    if (!gtk_builder_add_from_file ( builder, ui_filename, &error ))
    {
        g_warning ("gtk_builder_add_from_file failed: %s", error->message);
        g_error_free (error);
        return -1;
    }

    /* Obtain widgets that we need */
    guidialog.window = GTK_WIDGET ( gtk_builder_get_object( builder, "demo_gui" ));
    g_signal_connect (guidialog.window, "destroy", gtk_main_quit, NULL);

    guidialog.gui_cancel = GTK_WIDGET( gtk_builder_get_object( builder, "btn_cancel" ));
    g_signal_connect (guidialog.gui_cancel, "clicked",  gtk_main_quit, NULL);

    guidialog.gui_generate = GTK_WIDGET( gtk_builder_get_object( builder, "btn_generate" ));
    g_signal_connect (guidialog.gui_generate, "clicked", G_CALLBACK (generate_tree), NULL);


    guidialog.gui_sdldemo = GTK_WIDGET( gtk_builder_get_object( builder, "btn_sdldemo" ));
    g_signal_connect (guidialog.gui_sdldemo, "clicked", G_CALLBACK (run_sdldemo), NULL);

    guidialog.gui_sdldemo_surface = GTK_WIDGET( gtk_builder_get_object( builder, "btn_sdldemo_surface" ));
    g_signal_connect (guidialog.gui_sdldemo_surface, "clicked", G_CALLBACK (run_sdldemo_surface), NULL);

    guidialog.gui_width = GTK_WIDGET( gtk_builder_get_object( builder, "width_entry"));
    guidialog.gui_height = GTK_WIDGET( gtk_builder_get_object( builder, "height_entry"));

    guidialog.gui_seed = GTK_WIDGET( gtk_builder_get_object( builder, "seed_entry"));
    guidialog.gui_depth = GTK_WIDGET( gtk_builder_get_object( builder, "depth_entry"));

    /* Setup default values */
    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_width), "256");
    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_height), "512");
    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_seed), "23505");
    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_depth), "40");


    /* Destroy builder */
    g_object_unref (G_OBJECT( builder ));

    /* Show main window and start main loop */
    gtk_widget_show ( guidialog.window );

    gtk_main ();

    return(0);
}

