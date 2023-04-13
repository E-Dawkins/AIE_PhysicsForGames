using UnityEngine;

public class RandomFloat : StateMachineBehaviour
{
    [SerializeField] private string floatName;
    [SerializeField] private float minInclusive = 0;
    [SerializeField] private float maxInclusive = 0;

    public override void OnStateEnter(Animator _animator, AnimatorStateInfo _stateInfo, int _layerIndex)
    {
        // When animator enters this state, set float to random value
        _animator.SetFloat(floatName, Random.Range(minInclusive, maxInclusive));
    }
}
