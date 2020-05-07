//
//  SchedulingMinutesViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 3/14/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth
class SchedulingMinutesViewController: UIViewController {

    @IBOutlet weak var pickerView2: UIPickerView!
    @IBOutlet weak var minuteLabel: UILabel!
    
    var myPeripheral: CBPeripheral!
    var butChar: CBCharacteristic?
    
    private let dataSource = ["00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19", "20","21","22","23","24","25","26","27","28","29","30", "31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59"]
    
    var index: Int = 0
    
    override func viewDidLoad() {
        super.viewDidLoad()
        pickerView2.delegate = self
        pickerView2.dataSource = self
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "M"{
            let vc = segue.destination as! SchedulingStartViewController
            vc.butChar = self.butChar
            let per = segue.destination as! SchedulingStartViewController
            per.myPeripheral = self.myPeripheral
        }
    }
    
    @IBAction func minBut(_ sender: Any) {
        print("Minute Button Sending")
        let minString: String = dataSource[index]
        print(minString)
        let minData = Data(minString.utf8)
        myPeripheral.writeValue(minData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
}

extension SchedulingMinutesViewController: UIPickerViewDelegate, UIPickerViewDataSource{
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return dataSource.count
    }
    
    func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        minuteLabel.text = dataSource[row]
        index = row
    }
    
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return dataSource[row]
    }
}
