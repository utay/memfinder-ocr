#include <stdlib.h>
#include <stdio.h>
#include "../headers/gui.h"

typedef struct{
     GtkBuilder *builder;
     gpointer user_data;
  } SGlobalData;

GtkWidget *window, *image;
char *file;

int main(int argc, char *argv[])
{
  GtkWidget *mainwindow = NULL;
  SGlobalData data;
  //GtkBuilder *builder = NULL;
  GError *error = NULL;
  gchar *filename = NULL;
  gtk_init(&argc, &argv);
  data.builder = gtk_builder_new();
  filename =  g_build_filename ("extra/gui/memfinder.glade", NULL);
  gtk_builder_add_from_file (data.builder, filename, &error);
  g_free (filename);
  if (error)
  {
    gint code = error->code;
    g_printerr("%s\n", error->message);
    g_error_free (error);
    return code;
  }
  gtk_builder_connect_signals (data.builder, &data);
  mainwindow = GTK_WIDGET(gtk_builder_get_object (data.builder, "MainWindow"));
  g_signal_connect (G_OBJECT (mainwindow), "destroy", (GCallback)gtk_main_quit, NULL);
  gtk_widget_show_all (mainwindow);
  gtk_main();
  return 0;
}

char* get_file()
{
    char* filename = NULL;
  GtkWidget *dialog = gtk_file_chooser_dialog_new
    ("Open File",
    (GtkWindow *) window,
    GTK_FILE_CHOOSER_ACTION_OPEN,
    "_Cancel",
    GTK_RESPONSE_CANCEL,
    "_Open",
    GTK_RESPONSE_ACCEPT,
    NULL);
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_widget_destroy(dialog);
  }
  return filename;
}

void on_addMemeButton_clicked(GtkButton *buttonOpen, gpointer user_data)
{
  SGlobalData *data = (SGlobalData*) user_data;
  g_print("Loading file\n");
  file = get_file();
  image = gtk_image_new_from_file(file);
  gtk_widget_queue_draw(image);
  g_print("File Loaded\n");
  g_print("%s\n", file);
}

void on_loadButton_clicked(GtkButton *buttonOpen, gpointer user_data)
{
  SGlobalData *data = (SGlobalData*) user_data;
  GtkImage *imageShow;
  GdkPixbuf *pixbuf;
  GError *error = NULL;
  g_print("Loading file\n");
  file = get_file();
  pixbuf = gdk_pixbuf_new_from_file(file, &error);
  GdkPixbuf *pixbuf_mini = NULL;
  int width = gdk_pixbuf_get_width(pixbuf)/2;
  int height = gdk_pixbuf_get_height(pixbuf)/2;
  pixbuf_mini = gdk_pixbuf_scale_simple(pixbuf, width, height,
              GDK_INTERP_NEAREST);
  image = gtk_image_new_from_pixbuf(pixbuf_mini);
  gtk_widget_queue_draw(image);
  g_print("File Loaded\n");
  g_print("%s\n", file);
  imageShow = GTK_WIDGET(gtk_builder_get_object (data->builder,"memeImage"));
  gtk_image_set_from_pixbuf(imageShow,pixbuf_mini);
}

void on_OCRButton_clicked (GtkButton *OCRbutton, gpointer user_data)
{
  SGlobalData *data = (SGlobalData*) user_data;
  GtkWidget *dialog = NULL;
  dialog = GTK_WIDGET (gtk_builder_get_object (data->builder, "OCRDialog"));
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_hide (dialog);
}

void on_backButton_clicked (GtkButton *OCRbutton, gpointer user_data)
{
  SGlobalData *data = (SGlobalData*) user_data;
  GtkWidget *dialog = NULL;
  dialog =  GTK_WIDGET (gtk_builder_get_object (data->builder, "OCRDialog"));
  gtk_widget_hide (dialog);
}

void on_detectButton_clicked (GtkButton *OCRbutton, gpointer user_data)
{
  SGlobalData *data = (SGlobalData*) user_data;
  GtkWidget *dialog = NULL;
  gtk_widget_hide (dialog);
}

void on_button1_clicked (GtkButton *labelButton, gpointer user_data)
{
  SGlobalData *data = (SGlobalData*) user_data;
  GtkWidget *dialog = NULL;
  SDL_Surface *image = load_image(file);
  printf("%s\n",file);
  //textimgLabel = gtk_label_new(text[0]);
  gtk_button_set_label(GTK_BUTTON(labelButton), ocr(image));
  gtk_widget_hide (dialog);
}
