package com.example.esp32_control1

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Toast
import com.example.esp32_control1.databinding.ActivityMainBinding
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private lateinit var database: DatabaseReference
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        //setContentView(R.layout.activity_main)
        binding.buttonRead.setOnClickListener { readData() }
        binding.buttonSetLEDValue.setOnClickListener { setData() }
        binding.switch1.setOnClickListener { setData2() }
    }

    private fun setData() {
        var pwmValue = 0
        try {
            pwmValue = binding.editTextPWM.text.toString().toInt()

        } catch (e: Exception) {
            Toast.makeText(this, e.message, Toast.LENGTH_SHORT).show()
        }
        database = FirebaseDatabase.getInstance().getReference("LED")
        database.child("analog").setValue(pwmValue).addOnSuccessListener {
            Toast.makeText(this, "PWM Value set Succesfull", Toast.LENGTH_SHORT).show()
        }.addOnFailureListener {
            Toast.makeText(this, "FAILED to set a new PWM value", Toast.LENGTH_SHORT).show()
        }
    }
        private fun readData() {

            database = FirebaseDatabase.getInstance().getReference("sensor")
            database.child("voltage").get().addOnSuccessListener {
                if (it.exists()) {
                    var voltage: Float = it.value.toString().toFloat()
                    Toast.makeText(this, "Succesfull Voltage Read", Toast.LENGTH_SHORT).show()
                    binding.textViewVolts.setText(String.format("%.2f", voltage))
                } else {
                    Toast.makeText(this, "sensor/voltage path", Toast.LENGTH_SHORT).show()
                }
            }.addOnFailureListener {
                Toast.makeText(this, "FAILED", Toast.LENGTH_SHORT).show()
            }

        }

    private fun setData2() {
        var digitalValue = false
        try {

            digitalValue = binding.switch1.isChecked
        }catch(e:Exception){
            Toast.makeText(this, e.message, Toast.LENGTH_SHORT).show()
        }
        database.child("digital").setValue(digitalValue).addOnSuccessListener {
            Toast.makeText(this, "Digital Value set successfully", Toast.LENGTH_SHORT).show()
        }.addOnFailureListener {
            Toast.makeText(this, "Failed to set a new Digital value", Toast.LENGTH_SHORT).show()
        }

    }
}

