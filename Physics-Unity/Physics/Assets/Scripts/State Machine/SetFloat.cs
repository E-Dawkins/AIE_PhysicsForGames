using UnityEngine;

public class SetFloat : StateMachineBehaviour
{
	[SerializeField] private string floatName;
	[SerializeField] private float value;
	
	private enum SetWhen { OnEnter, OnExit };
	[SerializeField] private SetWhen setWhen;
    
	public override void OnStateEnter(Animator _animator, AnimatorStateInfo _stateInfo, int _layerIndex)
	{
		// When animator enters state, set float to value
		// only if setWhen is set to onEnter
		if (setWhen == SetWhen.OnEnter)
			_animator.SetFloat(floatName, value);
	}

	public override void OnStateExit(Animator _animator, AnimatorStateInfo _stateInfo, int _layerIndex)
	{
		// When animator exits state, set float to value
		// only if setWhen is set to onExit
		if (setWhen == SetWhen.OnExit)
			_animator.SetFloat(floatName, value);
	}
}
