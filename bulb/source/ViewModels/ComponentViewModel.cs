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

        public delegate void ModifyComponentHandler(string componentName, byte[] data);
        public ModifyComponentHandler OnModified;

        public delegate void RemoveComponentHandler(string typeName);
        public RemoveComponentHandler OnRemoved;

        private byte[] componentData;
        private Membrane.EditorTypeInfo componentTypeInfo;

        public ComponentViewModel(Membrane.EditorTypeInfo componentTypeInfo, byte[] componentData) {
            Name = componentTypeInfo.displayName;
            TypeName = componentTypeInfo.typeName;
            this.componentData = componentData;
            this.componentTypeInfo = componentTypeInfo;

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

            TypeViewModel vm;
            if (typeInfo.memberInfo != null && members.Count == 0) { //Count == 0 ensures we only do this for leaf members
                unsafe {
                    fixed (byte* memPtr = typeMemory) {
                        //TODO: We know we're using numbers here but what about other types such as strings?
                        float val;
                        Buffer.MemoryCopy(memPtr + typeInfo.memberInfo.offset, &val, sizeof(float), typeInfo.memberInfo.size);
                        
                        vm = new TypeViewModel(typeInfo.displayName, typeInfo.memberInfo.offset, typeInfo.memberInfo.size, val.ToString());
                        vm.OnValueChanged += OnValueChanged;
                    }
                }
            } else {
                vm = new TypeViewModel(typeInfo.displayName, members);
            }

            return vm;
        }

        private void OnValueChanged(int valueOffset, int valueSize, string value) {
            //TODO: Only handling numbers for now - other basic types need to be supported
            unsafe {
                fixed (byte* memPtr = componentData) {
                    //TODO: We know we're using numbers here but what about other types such as strings?
                    if (!float.TryParse(value, out float val)) {
                        Membrane.Log.write(Membrane.LogLevel.Error, "Could not parse value");
                    }

                    Buffer.MemoryCopy(&val, memPtr + valueOffset, valueSize, sizeof(float));

                    OnModified?.Invoke(componentTypeInfo.typeName, componentData);
                }
            }
        }
    }
}