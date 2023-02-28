using UnityEngine;

public class RandomInt : StateMachineBehaviour
{
    [SerializeField] private string intName;
    [SerializeField] private int lowerInclusive = 0;
    [SerializeField] private int upperExclusive = 0;

    public override void OnStateEnter(Animator _animator, AnimatorStateInfo _stateInfo, int _layerIndex)
    {
        _animator.SetInteger(intName, Random.Range(lowerInclusive, upperExclusive));
    }
}
