#include <gtk/gtk.h>

#include "config.h"

typedef struct
{
  GSimpleActionGroup *actions;
  GMenu *menu;

  guint actions_export_id;
  guint menu_export_id;
} IndicatorTestService;

static void
activate_about (GSimpleAction *action,
               GVariant      *parameter,
               gpointer       user_data)
{
  gtk_show_about_dialog(NULL,
                        "program-name", PROJECT_NAME,
                        "title", "About " PROJECT_NAME,
                        "version", PROJECT_VERSION_MAJOR "." PROJECT_VERSION_MINOR,
                        "license_type", GTK_LICENSE_GPL_3_0,
                        "wrap_license", TRUE,
                        "website", "https://github.com/sneetsher/mysystemindicator",
                        "website_label", "https://github.com/sneetsher/mysystemindicator",
                        "logo_icon_name", "indicator-" SHORT_NAME,
                        NULL);
  g_message ("showing about dialog");
}

static void
activate_private (GSimpleAction *action,
               GVariant      *parameter,
               gpointer       user_data)
{
  g_message ("clicked private menu entry");
}

static void
activate_exit (GSimpleAction *action,
               GVariant      *parameter,
               gpointer       user_data)
{
  g_message ("exit the program");
  gtk_main_quit();
}


static void
bus_acquired (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
  IndicatorTestService *indicator = user_data;
  GError *error = NULL;

  GMenuItem *item, *item_p;
  GMenu *submenu, *submenu_p, *menu_p;
    
  GActionEntry entries[] = {
    { "_header", NULL, NULL, "{'label': <'Test'>,"
                             " 'icon': <'indicator-test'>,"
                             " 'accessible-desc': <'Test indicator'> }", NULL },
    { "about", activate_about, NULL, NULL, NULL },
    { "private", activate_private, NULL, NULL, NULL },
    { "exit", activate_exit, NULL, NULL, NULL }
  };

  indicator->actions = g_simple_action_group_new ();
  g_simple_action_group_add_entries (indicator->actions, entries, G_N_ELEMENTS (entries), NULL);

  submenu = g_menu_new ();
  g_menu_append (submenu, "About", "indicator.about");
  g_menu_append (submenu, "Exit", "indicator.exit");
  item = g_menu_item_new (NULL, "indicator._header");
  g_menu_item_set_attribute (item, "x-canonical-type", "s", "com.canonical.indicator.root");
  g_menu_item_set_submenu (item, G_MENU_MODEL (submenu));
  indicator->menu = g_menu_new ();
  g_menu_append_item (indicator->menu, item);

  indicator->actions_export_id = g_dbus_connection_export_action_group (connection,
                                                                        DBUS_PATH,
                                                                        G_ACTION_GROUP (indicator->actions),
                                                                        &error);
  if (indicator->actions_export_id == 0)
    {
      g_warning ("cannot export action group: %s", error->message);
      g_error_free (error);
      return;
    }

  indicator->menu_export_id = g_dbus_connection_export_menu_model (connection,
                                                                   DBUS_PATH "/greeter",
                                                                   G_MENU_MODEL (indicator->menu),
                                                                   &error);
  if (indicator->menu_export_id == 0)
    {
      g_warning ("cannot export menu: %s", error->message);
      g_error_free (error);
      return;
    }
    
  submenu_p = g_menu_new ();
  g_menu_append (submenu_p, "About", "indicator.about");
  g_menu_append (submenu_p, "Private", "indicator.private");
  g_menu_append (submenu_p, "Exit", "indicator.exit");
  item_p = g_menu_item_new (NULL, "indicator._header");
  g_menu_item_set_attribute (item_p, "x-canonical-type", "s", "com.canonical.indicator.root");
  g_menu_item_set_submenu (item_p, G_MENU_MODEL (submenu_p));
  menu_p = g_menu_new ();
  g_menu_append_item (menu_p, item_p);
  indicator->menu_export_id = g_dbus_connection_export_menu_model (connection,
                                                                   DBUS_PATH "/desktop",
                                                                   G_MENU_MODEL (menu_p),
                                                                   &error);
  if (indicator->menu_export_id == 0)
    {
      g_warning ("cannot export menu: %s", error->message);
      g_error_free (error);
      return;
    }
    
  g_object_unref (submenu);
  g_object_unref (item);
  g_object_unref (submenu_p);
  g_object_unref (menu_p);
  g_object_unref (item_p);
}

static void
name_lost (GDBusConnection *connection,
           const gchar     *name,
           gpointer         user_data)
{
  IndicatorTestService *indicator = user_data;

  if (indicator->actions_export_id)
    g_dbus_connection_unexport_action_group (connection, indicator->actions_export_id);

  if (indicator->menu_export_id)
    g_dbus_connection_unexport_menu_model (connection, indicator->menu_export_id);
}


int
main (int argc, char **argv)
{
  gtk_init(&argc, &argv);

  IndicatorTestService indicator = { 0 };

  g_bus_own_name (G_BUS_TYPE_SESSION,
                  DBUS_NAME,
                  G_BUS_NAME_OWNER_FLAGS_NONE,
                  bus_acquired,
                  NULL,
                  name_lost,
                  &indicator,
                  NULL);

  gtk_main();
  
  g_object_unref (indicator.actions);
  g_object_unref (indicator.menu);


  return 0;
}
