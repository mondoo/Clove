using System.Windows.Input;
using System.Collections.ObjectModel;
using System.Collections.Generic;

using Membrane = membrane;
using System;

namespace Bulb {
    /// <summary>
    /// Viewmodel for displaying components in editor
    /// </summary>
    public class ComponentViewModel : ViewModel {
        public string Name { get; }
        public string TypeName { get; }

        public ObservableCollection<TypeViewModel> Members { get; }

        public ICommand RemoveComponentCommand { get; }

        public delegate void RemoveComponentHandler(string typeName);
        public RemoveComponentHandler OnRemoved;

        public ComponentViewModel(Membrane.EditorTypeInfo componentTypeInfo, byte[] componentData) {
            Name = componentTypeInfo.displayName;
            TypeName = componentTypeInfo.typeName;

            Members = new ObservableCollection<TypeViewModel>();
            foreach (var memberInfo in componentTypeInfo.members) {
                Members.Add(BuildTypeViewModel(memberInfo, componentData));
            }

            RemoveComponentCommand = new RelayCommand(() => { OnRemoved?.Invoke(TypeName); });
        }

        private TypeViewModel BuildTypeViewModel(Membrane.EditorTypeInfo typeInfo, byte[] typeMemory) {
            var members = new List<TypeViewModel>();
            foreach (var memberInfo in typeInfo.members) {
                members.Add(BuildTypeViewModel(memberInfo, typeMemory));
            }

            var vm = new TypeViewModel(typeInfo.displayName, members);
            if (typeInfo.memberInfo != null && members.Count == 0) { //Count == 0 ensures we only do this for leaf members
                unsafe {
                    fixed (byte* memPtr = typeMemory) {
                        //TODO: We know we're using numbers here but what about other types such as strings?
                        float val;
                        Buffer.MemoryCopy(memPtr + typeInfo.memberInfo.offset, &val, sizeof(float), typeInfo.memberInfo.size);
                        vm.Value = val.ToString();
                    }
                }
            }

            return vm;
        }
    }
}