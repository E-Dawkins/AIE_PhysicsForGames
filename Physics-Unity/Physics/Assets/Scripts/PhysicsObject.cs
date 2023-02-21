using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class PhysicsObject : MonoBehaviour
{
    [SerializeField] private Material awakeMat;
    [SerializeField] private Material asleepMat;

    private Rigidbody _rb = null;
    private bool _isSleeping;

    // Start is called before the first frame update
    private void Start()
    {
        _rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    private void FixedUpdate()
    {
        if(_rb.IsSleeping() && _isSleeping && asleepMat != null)
        {
            _isSleeping = false;
            GetComponent<MeshRenderer>().material = asleepMat;
        }

        if(!_rb.IsSleeping() && !_isSleeping && awakeMat != null)
        {
            _isSleeping = true;
            GetComponent<MeshRenderer>().material = awakeMat;
        }
    }
}
