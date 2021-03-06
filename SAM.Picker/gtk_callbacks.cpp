#include "gtk_callbacks.h"

// See comments in the header file

extern "C" 
{

    void 
    on_close_button_clicked() {
        gtk_main_quit();
        gtk_widget_destroy(g_main_gui->get_main_window());

        delete g_main_gui;
        g_main_gui = NULL;

        g_steam->quit_game();
    }
    // => on_close_button_clicked

    void
    on_store_button_clicked() {
        std::cerr << "Saving stats and achievements." << std::endl;
        const std::map<std::string, bool> pending_achs = g_steam->get_pending_ach_modifications();
        const std::map<std::string, double> pending_stats = g_steam->get_pending_stat_modifications();
        GameEmulator* emulator = GameEmulator::get_instance();
        char tmp_id[MAX_ACHIEVEMENT_ID_LENGTH];

        // TODO: JUST DO IT
        /**
         * foreach(pending_achs as &val) {
         * if(val.second == true)
         *  gameEmulator->unlock(val.first)
         * } else {
         *  gameEmulator->relock(val.first)
         * }
         * }
         */
        
        /**
         * TODO: Check for failures. But unlocking is done async because
         * the son process has to deal with it. 
         */
        for (auto const& [key, val] : pending_achs) {
            if(val) {
                std::cout << "Unlocking " << key << std::endl;
                strncpy(tmp_id, key.c_str(), MAX_ACHIEVEMENT_ID_LENGTH); // TODO REMOVE ME IT WAS FOR TESTING JUST USE C_STR INLINE
                emulator->unlock_achievement( tmp_id );
                g_steam->remove_modification_ach(key);
            } else {
                std::cout << "Relocking " << key << std::endl;
                emulator->relock_achievement( key.c_str() );
                g_steam->remove_modification_ach(key);                
            }
        }

        emulator->update_data_and_view(); // This is async
    }
    // => on_store_button_clicked

    void 
    on_ask_game_refresh() {
        g_main_gui->reset_game_list();
        g_steam->refresh_owned_apps();

        for(Game_t app : g_steam->get_all_games_with_stats()) {
            g_main_gui->add_to_game_list(app);
        }

        g_steam->refresh_icons();
        g_main_gui->confirm_game_list();
    }
    // => on_ask_game_refresh


    void 
    on_main_window_show() {
        on_ask_game_refresh(); //Run this async?
    }
    // => on_main_window_show


    void
    on_search_changed(GtkWidget* search_widget) {
        const char* filter_text = gtk_entry_get_text( GTK_ENTRY(search_widget) );

        //if !g_steam->isgamerunning
        g_main_gui->filter_games(filter_text);
        //else 
        //g_main_gui->filter_stats(filter_text)
    }
    // => on_search_changed


    void 
    on_game_row_activated(GtkListBox *box, GtkListBoxRow *row) {
        const std::string app_id( std::to_string( g_main_gui->get_corresponding_appid_for_row(row) ) );

        if( app_id != "0" ) {
            g_main_gui->switch_to_stats_page();
            g_steam->launch_game(app_id);
            
        } else {
            std::cerr << "An error occurred figuring out which app to launch.. You can report this to the developer." << std::endl;
        }

    }
    // => on_game_row_activated

    void
    on_back_button_clicked() {
        g_steam->quit_game();
        g_main_gui->switch_to_games_page();
    }
    // => on_back_button_clicked
}