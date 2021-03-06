#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <csignal>
#include <dirent.h>
#include "Game.h"
#include "SteamAppDAO.h"
#include "GameEmulator.h"
#include "../common/functions.h"

/**
 * MySteam is a class that aims to retrieve as much data as
 * possible regarding the latest logged in user on the machine.
 * This data includes owned games with app and achievements,
 * and identifiers.
 */
class MySteam {
public:
    /**
     * Returns the unique instance of this object.
     * See "Singleto design pattern" for further help
     */
    static MySteam* get_instance();

    /**
     * Returns the steamId3 of the last user who logged in on the
     * machine. Make sure all logs are enabled, or this may result
     * in an error.
     */
    static std::string get_user_steamId3();

    /**
     * Returns the absolute path to the steam installation folder.
     * This is not failsafe and may require some tweaking to add 
     * support for your distribution
     */
    static std::string get_steam_install_path();

    /**
     * Simple getter for m_pending_ach_modifications
     */
    std::map<std::string, bool> get_pending_ach_modifications() const { return m_pending_ach_modifications; };

    /**
     * Simple getter for m_pending_stat_modifications
     */
    std::map<std::string, double> get_pending_stat_modifications() const { return m_pending_stat_modifications; };

    /**
     * Starts a process that will emulate a steam game with the 
     * given appId. Returns false if this process failed to launch.
     * The process may start successfully but fail during execution.
     */
    bool launch_game(std::string appId);

    /**
     * Stops the process started with the above method launch_game.
     * Returns true if a process was successfully stopped.
     */
    bool quit_game();

    /**
     * Mostly used for debug purposes, prints all apps owned
     * (with stats or achievements) in the console.
     */
    void print_all_owned_games() const;

    /**
     * Makes a list of all owned games with stats or achievements.
     */
    void refresh_owned_apps();

    /**
     * Fetches all the app icons either online or on the disk.
     * Once fetched, the view is automatically updated with
     * the fetched image. (Observer pattern)
     */
    void refresh_icons();

    /**
     * Returns all the already loaded retrieved apps by the latest logged 
     * in user. Make sure to call refresh_owned_apps at least once to get 
     * correct results
     */
    std::vector<Game_t> get_all_games_with_stats() { return m_all_subscribed_apps; };

    /**
     * Adds a modification to be done on the launched app.
     * Commit the change with commit_modifications.
     */
    void add_modification_ach(const std::string& ach_id, const bool& new_value);

    /**
     * Adds a modification to be done on the launched app.
     * Commit the change with commit_modifications.
     */
    void remove_modification_ach(const std::string& ach_id);

    /**
     * Adds a modification to be done on the launched app.
     * Commit the change with commit_modifications.
     */
    //void add_modification_stat(const std::string& stat_id, const double& new_value); // TODO: IMPLEMENT

    MySteam(MySteam const&)                 = delete;
    void operator=(MySteam const&)          = delete;
private:
    MySteam();

    std::vector<Game_t> m_all_subscribed_apps;
    std::map<std::string, bool> m_pending_ach_modifications;
    std::map<std::string, double> m_pending_stat_modifications;
};