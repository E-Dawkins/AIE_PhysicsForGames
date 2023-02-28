using UnityEngine;

public class SetFloat : StateMachineBehaviour
{
	[SerializeField] private string floatName;
	[SerializeField] private float amount;
	
	public override void OnStateEnter(Animator _animator, AnimatorStateInfo _stateInfo, int _layerIndex)
	{
		_animator.SetFloat(floatName, amount);
	}
}
