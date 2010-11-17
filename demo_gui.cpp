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
    GtkWidget *gui_generate;

    /* Share informations */
    GtkWidget *gui_width;
    GtkWidget *gui_height;

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

    //Prevzato z demo.cpp
    const size_t width = atoi(width_str);
	const size_t height = atoi(height_str);

	char asciiart[height][width];
	//char *asciiart = new char[int(height)];
	ofstream xpm;
	xpm.open("tree.xpm");
	xpm << "/* XPM */\n";
	xpm << "static char * tree[] = {\n";
	xpm << '"' << width << ' ' << height << " 2 1\",\n";
	xpm << "\"# c #000000\",\n";
	xpm << "\". c #ffffff\",\n";
	GenTreeBillboardTexture((char*)asciiart, width, height, 23505);
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

    guidialog.gui_width = GTK_WIDGET( gtk_builder_get_object( builder, "width_entry"));
    guidialog.gui_height = GTK_WIDGET( gtk_builder_get_object( builder, "height_entry"));



    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_width), "256");
    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_height), "512");


    /* Destroy builder */
    g_object_unref (G_OBJECT( builder ));

    /* Show main window and start main loop */
    gtk_widget_show ( guidialog.window );

    gtk_main ();

    return(0);
}
