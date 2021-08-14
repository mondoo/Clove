using System.IO;

namespace Garlic.Bulb {
    public class FileViewModel : DirectoryItemViewModel {
        public override ObjectType Type => ObjectType.File;

        public FileViewModel(FileInfo file, DirectoryItemViewModel parent)
            : base(parent) {
            Name = file.Name;
            VfsPath = ConvertSystemPathToVfsPath(file.FullName);
        }
    }
}