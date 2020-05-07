//
//  SchedulingViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 3/14/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth

class SchedulingViewController: UIViewController {
    
    private let dataSource = ["6 AM","7 AM", "8 AM", "9 AM", "10 AM", "11 AM", "12 PM", "1 PM", "2 PM", "3 PM", "4 PM", "5 PM", "6 PM", "7 PM", "8 PM", "9 PM", "10 PM", "11 PM"]
    
    private let bleSource = ["06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"]
    
    @IBOutlet weak var pickerView: UIPickerView!
    @IBOutlet weak var detailedLabel: UILabel!
    var index: Int = 0
    
    override func viewDidLoad() {
        super.viewDidLoad()
        pickerView.delegate = self
        pickerView.dataSource = self

    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    var myPeripheral: CBPeripheral!
    var butChar: CBCharacteristic?
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "H"{
            let vc = segue.destination as! SchedulingMinutesViewController
            vc.butChar = self.butChar
            let per = segue.destination as! SchedulingMinutesViewController
            per.myPeripheral = self.myPeripheral
        }
    }
    @IBAction func hourBut(_ sender: UIButton) {
        print("Hour Button Sending")
        let hourString: String = bleSource[index]
        print(hourString)
        let hourData = Data(hourString.utf8)
        myPeripheral.writeValue(hourData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
}

extension SchedulingViewController: UIPickerViewDelegate, UIPickerViewDataSource{
    
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return dataSource.count
    }
    
    func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        detailedLabel.text = dataSource[row]
        index = row
    }
    
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return dataSource[row]
    }
     
}
