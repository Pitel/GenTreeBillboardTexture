#include <gtk/gtk.h>
#include <glib.h>
#include <cstring>
#include <cstdio>
#include <string>

#include "GenTreeBillboardTexture.h"
#include <iostream>
#include <fstream>
#include <sstream>


/** Dialog widgets */
typedef struct _guidialog
{
    GtkWidget  *window;

    GtkWidget *gui_cancel;
    GtkWidget *gui_sdldemo_surface;

    /* Share informations */
    GtkWidget *gui_treetype;
    GtkWidget *gui_width;
    GtkWidget *gui_height;
    GtkWidget *gui_seed;
    GtkWidget *gui_depth;
    GtkWidget *gui_trunk;
    GtkWidget *gui_leaf;

} GuiDialog;


typedef enum treetyp
{
    LISTNATY = 0,
    JEHLICNAN
} TShare_service;

/** Dialog window */
GuiDialog guidialog;

GtkWidget *colorseldlg = NULL;
GtkWidget *drawingarea = NULL;
GdkColor color;
GdkColor trunk_color = {0, 90*255,60*255,30*255};
GdkColor leaf_color = {0,30*255,150*255,30*255};

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


std::string IntToChar(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

void
color_select (GtkWidget *widget, gpointer data)
{
  gint response;
  GtkColorSelection *colorsel;

  GdkColor *tmp = (GdkColor *)data;
       /* Create color selection dialog */
      if (colorseldlg == NULL)
        colorseldlg = gtk_color_selection_dialog_new ("Select color");

     //   cout << "SELEEECT" << endl;
      /* Get the ColorSelection widget */
      colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (colorseldlg)->colorsel);

      gtk_color_selection_set_previous_color (colorsel, tmp);
      gtk_color_selection_set_current_color (colorsel, tmp);
      gtk_color_selection_set_has_palette (colorsel, TRUE);

      /* Show the dialog */
      response = gtk_dialog_run (GTK_DIALOG (colorseldlg));

      if (response == GTK_RESPONSE_OK)
      {
        gtk_color_selection_get_current_color (colorsel, tmp);
      /*  cout << (tmp->red) << endl;
        cout << (tmp->green) << endl;
        cout << (tmp->blue) << endl;

        cout << IntToChar(tmp->red >> 8) << endl;
        cout << IntToChar(tmp->green >> 8) << endl;
        cout << IntToChar(tmp->blue >> 8) << endl;*/


        gtk_widget_modify_bg ( GTK_WIDGET (guidialog.gui_trunk), GTK_STATE_NORMAL, &trunk_color);
        gtk_widget_modify_bg ( GTK_WIDGET (guidialog.gui_leaf), GTK_STATE_NORMAL, &leaf_color);
      }

      gtk_widget_hide (colorseldlg);
}

void
run_sdldemo_surface ( void )
{
    const gchar *width_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_width));
    const gchar *height_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_height));
    const gchar *seed_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_seed));
    const gchar *depth_str = gtk_entry_get_text ( GTK_ENTRY (guidialog.gui_depth));

    std::string t_r = IntToChar(trunk_color.red >> 8);
    std::string t_g = IntToChar(trunk_color.green >> 8);
    std::string t_b = IntToChar(trunk_color.blue >> 8);
    std::string l_r = IntToChar(leaf_color.red >> 8);
    std::string l_g = IntToChar(leaf_color.green >> 8);
    std::string l_b = IntToChar(leaf_color.blue >> 8);

    const gchar *gui_argv[13];
    GError *error;
/*
 cout << "RUN" << endl;
    cout << IntToChar(trunk_color.red) << endl;
        cout << IntToChar(trunk_color.green) << endl;
        cout << IntToChar(trunk_color.blue) << endl;

    cout << IntToChar(leaf_color.red) << endl;
        cout << IntToChar(leaf_color.green) << endl;
        cout << IntToChar(leaf_color.blue) << endl;

*/
    gui_argv[0] = "demo_sdl";
    gui_argv[1] = width_str;
    gui_argv[2] = height_str;
    gui_argv[3] = seed_str;
    gui_argv[4] = depth_str;
    gui_argv[5] = t_r.c_str();
    gui_argv[6] = t_g.c_str();
    gui_argv[7] = t_b.c_str();
    gui_argv[8] = l_r.c_str();
    gui_argv[9] = l_g.c_str();
    gui_argv[10] = l_b.c_str();

    if(gtk_combo_box_get_active (GTK_COMBO_BOX (guidialog.gui_treetype)) != JEHLICNAN )
        gui_argv[11] = "listnaty";
    else
        gui_argv[11] = "jehlicnan";

    gui_argv[12] = NULL;


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


    guidialog.gui_sdldemo_surface = GTK_WIDGET( gtk_builder_get_object( builder, "btn_sdldemo_surface" ));
    g_signal_connect (guidialog.gui_sdldemo_surface, "clicked", G_CALLBACK (run_sdldemo_surface), NULL);

    guidialog.gui_width = GTK_WIDGET( gtk_builder_get_object( builder, "width_entry"));
    guidialog.gui_height = GTK_WIDGET( gtk_builder_get_object( builder, "height_entry"));

    guidialog.gui_seed = GTK_WIDGET( gtk_builder_get_object( builder, "seed_entry"));
    guidialog.gui_depth = GTK_WIDGET( gtk_builder_get_object( builder, "depth_entry"));


    guidialog.gui_trunk = GTK_WIDGET( gtk_builder_get_object( builder, "btn_trunk"));
    g_signal_connect (guidialog.gui_trunk, "clicked", G_CALLBACK (color_select), (gpointer) &trunk_color);


    guidialog.gui_leaf = GTK_WIDGET( gtk_builder_get_object( builder, "btn_leaf"));
    g_signal_connect (guidialog.gui_leaf, "clicked", G_CALLBACK (color_select), (gpointer) &leaf_color);


    guidialog.gui_treetype = GTK_WIDGET( gtk_builder_get_object( builder, "combobox_treetype"));


    /* Setup default values */
    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_width), "600");
    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_height), "800");
    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_seed), "23505");
    gtk_entry_set_text( GTK_ENTRY (guidialog.gui_depth), "100");

    //gdk_color_parse ("brown", &trunk_color);
    //gdk_color_parse ("green", &leaf_color);

    gtk_widget_modify_bg ( GTK_WIDGET (guidialog.gui_trunk), GTK_STATE_NORMAL, &trunk_color);
    gtk_widget_modify_bg ( GTK_WIDGET (guidialog.gui_leaf), GTK_STATE_NORMAL, &leaf_color);


    gtk_combo_box_set_active (GTK_COMBO_BOX (guidialog.gui_treetype), LISTNATY);


    /* Destroy builder */
    g_object_unref (G_OBJECT( builder ));

    /* Show main window and start main loop */
    gtk_widget_show ( guidialog.window );

    gtk_main ();

    return(0);
}

