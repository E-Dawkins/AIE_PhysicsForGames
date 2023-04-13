using UnityEngine;

public class RandomInt : StateMachineBehaviour
{
    [SerializeField] private string intName;
    [SerializeField] private int minInclusive = 0;
    [SerializeField] private int maxExclusive = 0;

    public override void OnStateEnter(Animator _animator, AnimatorStateInfo _stateInfo, int _layerIndex)
    {
        // when animator enters state, set int to random value
        _animator.SetInteger(intName, Random.Range(minInclusive, maxExclusive));
    }
}
