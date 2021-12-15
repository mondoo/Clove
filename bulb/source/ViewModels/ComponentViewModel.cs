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

        public delegate void ModifyComponentHandler(string componentName, string memberName, string memberValue);
        public ModifyComponentHandler OnModified;

        public delegate void RemoveComponentHandler(string typeName);
        public RemoveComponentHandler OnRemoved;

        private readonly Membrane.EditorTypeInfo componentTypeInfo;

        public ComponentViewModel(Membrane.EditorTypeInfo componentTypeInfo) {
            Name = componentTypeInfo.displayName;
            TypeName = componentTypeInfo.typeName;
            this.componentTypeInfo = componentTypeInfo;

            Members = new ObservableCollection<TypeViewModel>();
            foreach (var memberInfo in componentTypeInfo.members) {
                Members.Add(BuildTypeViewModel(memberInfo));
            }

            RemoveComponentCommand = new RelayCommand(() => { OnRemoved?.Invoke(TypeName); });
        }

        private TypeViewModel BuildTypeViewModel(Membrane.EditorTypeInfo typeInfo) {
            var members = new List<TypeViewModel>();
            if (typeInfo.members != null) {
                foreach (Membrane.EditorTypeInfo memberInfo in typeInfo.members) {
                    members.Add(BuildTypeViewModel(memberInfo));
                }
            }

            TypeViewModel vm;
            if (typeInfo.value != null && members.Count == 0) {
                vm = new TypeViewModel(typeInfo.displayName, typeInfo.typeName, typeInfo.value);
                vm.OnValueChanged += OnValueChanged;
            } else {
                vm = new TypeViewModel(typeInfo.displayName, typeInfo.typeName, members);
            }

            return vm;
        }

        private void OnValueChanged(string memberName, string value) {
            OnModified?.Invoke(componentTypeInfo.typeName, memberName, value);
        }
    }
}