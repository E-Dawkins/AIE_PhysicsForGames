using UnityEngine;

public class SetBool : StateMachineBehaviour
{
    [SerializeField] private string boolName;
    [SerializeField] private bool value;

    private enum SetWhen { OnEnter, OnExit };
    [SerializeField] private SetWhen setWhen;
    
    public override void OnStateEnter(Animator _animator, AnimatorStateInfo _stateInfo, int _layerIndex)
    {
        if (setWhen == SetWhen.OnEnter)
            _animator.SetBool(boolName, value);
    }

    public override void OnStateExit(Animator _animator, AnimatorStateInfo _stateInfo, int _layerIndex)
    {
        if (setWhen == SetWhen.OnExit)
            _animator.SetBool(boolName, value);
    }
}
