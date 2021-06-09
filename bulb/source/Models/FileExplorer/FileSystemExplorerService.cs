using System.Linq;
using System.IO;
using System.Collections.Generic;
using Membrane = garlic.membrane;
using System;

//TODO: This will have to be refactored to work through the VFS and only know about directories that it is aware of.
//Leaving it like this for now just to follow the tutorial. Luckily because MVVM just this will change.

/*
Or would it? The editor will be mounting the the VFS locations so it'll know. the VFS is purely there for the engine's benefit.
-- it can just resolve the content location and go from there.
*/

namespace Garlic.Bulb {
    /// <summary>
    /// Gets file system information.
    /// </summary>
    public class FileSystemExplorerService {
        /// <summary>
        /// Gets a list of files in a given directory.
        /// </summary>
        /// <param name="directory">Where to get the files from.</param>
        /// <returns>Returns a List of files for the directory. List will be empty if exception is raised.</returns>
        public static IList<FileInfo> GetChildFiles(string directory) {
            try {
                return (from x in Directory.GetFiles(directory) select new FileInfo(x)).ToList();
            } catch (Exception e) {
                Membrane.Log.write(Membrane.LogLevel.Error, $"Could not load directory: {directory}. {e.Message}");
                return new List<FileInfo>();
            }
        }

        /// <summary>
        /// Gets a list of directories in a given directory.
        /// </summary>
        /// <param name="directory">Where to get the directories from.</param>
        /// <returns>Returns a List of directories for the directory. List will be empty if exception is raised.</returns>
        public static IList<DirectoryInfo> GetChildDirectories(string directory) {
            try {
                return (from x in Directory.GetDirectories(directory) select new DirectoryInfo(x)).ToList();
            } catch (Exception e) {
                Membrane.Log.write(Membrane.LogLevel.Error, $"Could not load directory: {directory}. {e.Message}");
                return new List<DirectoryInfo>();
            }
        }
    }
}