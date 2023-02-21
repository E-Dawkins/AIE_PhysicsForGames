using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class CannonShot : MonoBehaviour
{
    [SerializeField] private float fireForce = 400;
    
    private bool _canFire = true;
    private Rigidbody _rb;

    // Start is called before the first frame update
    private void Awake()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true;
    }

    // Update is called once per frame
    private void Update()
    {
        if(Input.anyKeyDown && _canFire)
        {
            _rb.isKinematic = false;
            _rb.AddForce(transform.forward * fireForce);
            _canFire = false;
        }

        if(_rb.IsSleeping())
            _canFire = true;
    }
}
